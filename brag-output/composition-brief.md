# Hyperframes Composition Brief: 明日方舟線索交換器

## Objective
Create a ~22.5s deadpan yc-parody launch video for an Arknights clue-exchange Discord bot whose punchline is that it really does brute-force the optimal trade in C++.

## Output
- Composition directory: `brag-output/composition/`
- Rendered video: `brag-output/brag.mp4`
- Format: landscape — 1920x1080
- Duration: ~22.5 seconds

## Source Material
- Project root: repository root of this Git repository
- Primary files read: README.md, cogs/exchange.py, cogs/clue.py, utils/crud_clues.py, utils/exchange/input.txt, exchange_algo/main.cpp
- Product name: 明日方舟線索交換器 (Arknights Clue Exchanger)
- Tagline / strongest claim: 把每日換線索，變成一行指令。/ 1,855 種組合，用 C++ 暴力解。
- Key UI to recreate: Discord dark-theme channel (clue messages stacking) + Discord code block printing the optimal trade arrows; a `/exchange` slash-command chip + 「正在輸入…」 typing indicator.
- Copy that must appear verbatim:
  - Clue strings: `123567 123567`, `1234 134`, `11345666 0`, `12222337 1`, `122334456 5`, `123 123`, `123 0`, `123 123`
  - Bot lines: `開始計算！`, `計算完成！`
  - Command: `/exchange`
  - Title beats: `8 名玩家。` `7 種線索。` `1 個最佳解。`
  - Reveal: `1,855 種組合` / `用 C++ 暴力解`
  - Caption: `人類算不出來。`
  - Logo/tagline: `明日方舟線索交換器` / `把每日換線索，變成一行指令。` / `ver 1.2`
  - Result rows (format `Name → Name : clue#`): `Player2 → Player1 : 4`, `Player3 → Player1 : 7`, `Player5 → Player2 : 1`, `Player4 → Player6 : 3`

## Creative Direction
- Tone preset: yc-parody (deadpan restraint)
- Creative direction: fake infrastructure-startup launch for an Arknights clue bot
- Interpretation: structured, straight-faced, big declarative type, hard cuts, no jokey motion; humor from over-engineering theater.
- Angle: a daily mobile-game chore solved with provably-optimal C++ combinatorics, pitched like a Series A.
- Hook: black card, hazard-yellow `8 名玩家。/ 7 種線索。/ 1 個最佳解。` one line at a time on the beat.
- Outro / punchline: 明日方舟線索交換器 lockup + tagline + dry `ver 1.2`.
- Avoid: generic SaaS language, abstract filler visuals, unrelated redesign, zany comedy.

## Visual Identity
- Background: chat `#313338`, sidebar `#2b2d31`, title cards `#1a1b1e`
- Text: body `#dbdee1`, headings `#f2f3f5`, muted `#949ba4`
- Accent: hazard yellow `#f5d130`; blurple `#5865F2` (command chip); success green `#23a55a`
- Display font: bold condensed sans (Inter / system); Body: system sans; Code/clues/result: monospace (ui-monospace)
- Visual references: Discord channel rows w/ avatars, slash-command chip, typing indicator dots, ``` code block result.

## Storyboard
Use `brag-output/brag-plan.md` as the creative contract.

Scene summary:
1. Title card hook — 3.7s — three hazard-yellow lines slam in one at a time; final lands on 3.70s cue.
2. The noise — 4.3s — 8 verbatim clue strings post one by one in a Discord channel; caption 「人類算不出來。」
3. Command + reveal — 4.95s — type `/exchange`, 「開始計算！」 + typing dots, then 「1,855 種組合 / 用 C++ 暴力解」 on 12.65s cue.
4. The result — 4.6s — code block prints 4 trade arrows one per beat, then 「計算完成！」 flips green.
5. Logo / punchline — 1.95s — 明日方舟線索交換器 lockup + tagline + `ver 1.2`; music fades.

## Audio
- Audio role: sparse professional accents over warm business-y bed (restrained yc-parody)
- Audio arc: warm bed under declarative title beats → accumulating message pops → key ticks + dry compute hit → placed-card ticks + one success accent → fade under logo.
- Music: `happy-beats-business-moves-vol-11-by-ende-dot-app.mp3` at volume 0.30, fade-out under final logo.
- Music cue guidance: bundled preset `assets/music/cues/happy-beats-business-moves-vol-11-by-ende-dot-app.music-cues.json` (114.84 BPM). Strong-cue locks (use 3): hook final line ~3.70s, compute reveal ~12.65s, logo ~17.91s. Beat grid for sequential clue posts 5.28/5.80/6.34/6.86/7.38/7.91 and result arrows 13.18/13.70/14.22/14.76.
- Audio-reactive treatment: subtle; hazard-yellow title glow + typing dots breathe with RMS. No waveform/equalizer.
- Audio-coupled moments:
  - Scene 1 title lines — beat-aligned reveal (1.60/2.65/3.70)
  - Scene 2 clue messages — sequential message pops on beat grid
  - Scene 3 `/exchange` — key ticks; combination count — dry tick
  - Scene 4 trade arrows — card-place per row; 計算完成！ — one success accent
  - Scene 5 logo — single settle accent, then music fade
- SFX selection guidance: soft, competent palette — `interface/drop_*` or `casino/card-place-*` for pops/arrows, `keyboard/keypress-*` randomized for the typed command, `impact/impactSoft_medium_*` for the dry compute hit, `impact/impactBell_heavy_000` for the success accent. Sparse; nothing above 0.8; nothing zany.
- SFX analysis guidance: read `sfx-analysis.md`; prefer low HF-risk files for repeated pops/arrows.
- Exact SFX choice: Hyperframes picks filenames/timestamps/density after the animation exists.
- Audio files: copy chosen music + SFX into `brag-output/composition/assets/`.

## Hyperframes Instructions
Use the current hyperframes CLI workflow (init / lint / inspect / render). Single 1920x1080 composition, GSAP timeline registered on `window.__timelines["main"]` (keyed by the root element's `data-composition-id`), paused. Show real Discord UI + verbatim copy. Keep all CJK text legible (hold each readable line ≥0.8s). Beat-lock 3 major reveals within ±0.15s; snap sequential pops/arrows to the beat grid within ±0.10s but hold the full set afterward so rows stay readable. Lint + inspect before render.
