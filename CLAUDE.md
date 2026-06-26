# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A Discord bot (discord.py) that helps a fixed group of Arknights players optimize daily "clue" (線索) exchanges. Players post their clue holdings in a Discord channel; the bot records them, runs a C++ optimizer, and announces the best exchange strategy. The core optimization algorithm is a separate C++ program (`exchange_algo/main.cpp`) invoked as a subprocess — not Python.

The codebase and most comments/strings are in Traditional Chinese.

## Commands

```bash
# Install deps (only discord.py + pydantic-settings[toml])
pip install -r requirements.txt

# Run the bot (requires config.toml + a compiled binary in utils/exchange/)
python main.py

# Build & run via Docker (compiles the C++ binary in a build stage)
docker build -t arknights-bot:0.1.0 .
docker run --rm -v path/to/config.toml:/app/config.toml:ro arknights-bot:0.1.0

# Compile + interactively test the C++ optimizer alone
docker build -f Dockerfile.exchange -t exchange-main-test .
docker run --rm -it --entrypoint /bin/bash exchange-main-test   # then: ./main
```

There is **no test suite, linter, or formatter** configured.

## Required setup before running

1. Copy `example.toml` → `config.toml` and fill in `discord.guild_id`, `discord.clue_channel_id`, `discord.info_channel_id`, `discord.bot.token`, and the `[[users]]` list (each user's Discord user `id` + display `name`).
2. A compiled optimizer binary must exist at `utils/exchange/main.exe` (Windows) or `utils/exchange/main` (Linux). The Dockerfile compiles `exchange_algo/main.cpp` automatically; for local non-Docker dev you must compile it yourself. `main.exe` is checked into the repo.

## Architecture

**Entry point** (`main.py`): subclasses `commands.Bot`, loads three cogs, copies global app commands to the configured guild and syncs them. On startup it calls `ClueProcessor.sync_user_data_from_config()` to regenerate `utils/exchange/_user_data.txt` from `config.toml` so the player roster stays in sync with the C++ input.

**Config** (`config.py`): pydantic-settings models sourced *only* from `config.toml` (env/dotenv sources are intentionally disabled in `settings_customise_sources`).

**Cogs** (`cogs/`):
- `clue.py` — collects clue data. Listens to `on_message`/`on_message_edit`/`on_message_delete` in the clue channel and updates records; provides `/set_clue` (set another player's clues), `/update` (rebuild from the last day of channel history), `/detail`. Every mutation echoes the full record to the info channel.
- `exchange.py` — `/exchange` runs the optimizer; `/result` re-posts the last result. If not everyone has updated today (`check_update_date`), it prompts with confirm/cancel buttons before computing. The C++ subprocess is run via `loop.run_in_executor` so it never blocks the event loop, and a Discord "typing…" indicator is shown during compute. `is_calculating` guards against concurrent runs.
- `tutorial.py` — `/tutorial` static help embed.

**Core logic** (`utils/crud_clues.py`): `ClueProcessor` is the bridge between Discord and the C++ optimizer. It is entirely static methods operating on flat text files in `utils/exchange/`. This is the key module to understand.

### Data flow / file contract (`utils/exchange/`)

The Python side and the C++ binary communicate through plain text files in this directory. Filenames are hardcoded in **both** `crud_clues.py` and `exchange_algo/main.cpp` — changing one requires changing the other.

- `_user_data.txt` — roster: line 1 header, line 2 header, then one player name per line. **Regenerated from config on every startup.** (`ClueProcessor.get_users()` skips the first two lines.)
- `input.txt` — clue holdings, one row per player (same order as roster). Each row is `"<owned> <wanted>"` clue digits (e.g. `123567 123567`).
- `detail.txt` — human-readable record with per-player last-update date; used by `check_update_date()` to detect who hasn't updated today.
- `_comb_data.txt` — static precomputed combination tables read by the C++ program; do not regenerate.
- `results.txt` — written by the C++ program, read back by `get_result()`. Gitignored.
- `_error_log.txt` — C++ error output. Gitignored.

### Encoding gotcha (important)

The C++ binary emits **Big5/CP950** text. Files written *by the optimizer* (`results.txt`, and `_user_data.txt` when read back) are read in Python with `encoding="cp950", errors="replace"`. Files written *by Python* (`input.txt`, `detail.txt`) use the default encoding. Preserve these encodings when editing I/O code or you'll get `UnicodeDecodeError` / mojibake.

### Clue format

A clue string is `"<owned_digits> <wanted_digits>"` using digits 0–7 (e.g. `1234 134`); `format_clue()` normalizes a single token to `"<token> 0"`, and `validate_clue()` enforces the `^[0-7]+( [0-7]+)?$` pattern. Deleting a message resets that player to `"0 0"`.

### Hardcoded special case

`handle_clue_message()` branches on a hardcoded Discord user id (`525463925194489876`) to parse a multi-line "set clues for multiple players" message format. Be aware this is user-specific, not generic.

## CI / release

`.github/workflows/docker-publish.yml` builds and pushes a multi-stage image to GHCR on pushes to `main` (when relevant paths change) and on `v*.*.*` tags. Tagging a release is the deploy mechanism.
