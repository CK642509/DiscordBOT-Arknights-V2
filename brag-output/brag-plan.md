# Brag Plan: 明日方舟線索交換器 (Arknights Clue Exchanger Bot)

## What is this app?
A Discord bot that runs a hand-written C++ combinatorial optimizer to compute the **provably optimal** clue-trading plan for a group of Arknights players — so eight people stop guessing who should give clue #4 to whom, and just read the answer the bot posts.

## The angle
Take a daily mobile-game chore — "who trades which clue to maximize rewards" — and treat the solution with the gravity of a Series A infrastructure launch. The joke is that it's *real*: there genuinely is a C++ engine brute-forcing 1,855 seven-clue combinations behind a `/exchange` slash command. Deadpan startup energy applied to Arknights clue logistics. We don't wink at the camera; we pitch it straight, and the absurdity carries itself.

## Hook (first 2-3 seconds)
Black screen, hazard-yellow type slamming in one line at a time on the beat:
**「8 名玩家。」 → 「7 種線索。」 → 「1 個最佳解。」**
(8 players. 7 clues. 1 optimal solution.) Cold, declarative, oversized — a pitch deck title card.

## Key moments (the middle)
- **The noise:** a Discord clue channel where eight players dump cryptic strings (`123567 123567`, `1234 134`, `11345666 0`…) one by one. It looks like chaos. Caption: 「人類算不出來。」 (Humans can't solve this.)
- **The command + the over-engineering reveal:** someone types `/exchange`. Bot replies 「開始計算！」 with a live "正在輸入…" typing indicator — then the deadpan flex: **「1,855 種組合 · 用 C++ 暴力解。」** (1,855 combinations. Brute-forced in C++.)
- **The payoff:** the actual result block prints, trade arrows arriving one by one — `Player2 → Player1 : 4`, `Player3 → Player1 : 7`, `Player5 → Player2 : 1` — then 「計算完成！」 flips green.

## Outro / punchline
Product lockup: **明日方舟線索交換器**. Tagline 「把每日換線索，變成一行指令。」 (Turns the daily clue trade into one command.) Deadpan kicker, small and dry: `ver 1.2`.

## User flow worth showing
entry → key action → result, recreated as real Discord UI:
1. Players post their clues in the clue channel (the cryptic number strings, verbatim from `input.txt`).
2. A player runs `/exchange`; the bot shows 「開始計算！」 → typing indicator → 「計算完成！」.
3. The optimal trade plan posts back as a monospace code block of `A → B : clue#` rows (verbatim format from `main.cpp`'s `results.txt` writer).

## Tone
- Preset: yc-parody (with deadpan restraint)
- Creative direction: a fake infrastructure-startup launch for an Arknights clue bot
- Interpretation: structured, confident, straight-faced delivery; hard cuts, big declarative type, no jokey motion. The humor is in applying serious engineering theater to a mobile-game side quest. Quiet, tasteful audio — never zany.

## Format: landscape — 1920x1080
## Duration: ~22.5 seconds

## Visual identity (from the project)
- Background: Discord dark `#313338` (chat), sidebar `#2b2d31`, near-black title cards `#1a1b1e`
- Accent: Arknights hazard yellow `#f5d130` (brand-industrial), Discord blurple `#5865F2` for command chips, success green `#23a55a` for 「計算完成！」
- Text: `#dbdee1` (Discord body), `#f2f3f5` (headings), muted `#949ba4`
- Display font: bold condensed sans (Inter / system fallback) for pitch-deck title cards
- Body font: system sans for Discord UI; **monospace** (ui-monospace / JetBrains-style) for the clue strings and the result code block — these live in ``` blocks in the real bot
- Strongest visual element: the result block of `A → B : clue#` trade arrows, and the cryptic clue strings stacking in the channel

## Share copy (draft)
I wrote a Discord bot that brute-forces the *mathematically optimal* Arknights clue trade in C++. Eight players, seven clues, one command: `/exchange`. 🟡

## Audio direction
- Role: sparse professional accents over a warm, business-y bed — restrained yc-parody
- Music: `happy-beats-business-moves-vol-11-by-ende-dot-app.mp3` (warm, business-y, 114.84 BPM)
- Music treatment: start at 0, volume ~0.3, gentle fade-out under the final logo; let major reveals land near strong cues
- Music cue guidance: bundled preset `assets/music/cues/happy-beats-business-moves-vol-11-by-ende-dot-app.music-cues.json`. Strong cues to target: hook line ~3.70s, compute reveal ~12.65s, logo ~17.91s. Beat grid for sequential clue messages and result arrows: 5.28, 5.80, 6.34, 6.86, 7.38, 7.91 (clues) and 13.18, 13.70, 14.22, 14.76 (arrows).
- Audio-reactive treatment: subtle; let the hazard-yellow title glow and the typing-indicator dots breathe slightly with RMS. No waveform/equalizer visuals.
- SFX posture: sparse, motion-matched. Soft message-pop per clue post, a couple of key ticks on the `/exchange` command, a dry processing tick on the combination count, light card-place per result arrow, one clean success accent on 「計算完成！」.
- Audio-coupled moments: stacking clue messages (sequential pops), typed `/exchange` (key ticks), combination count-up (tick), result arrows (card-place per row), success flip (accent).
- Restraint rule: no zany/comedic SFX, no dense stacking, nothing above 0.8. The audio sells *competence*, which is what makes the deadpan land.

## Storyboard

### Scene 1 — Title card hook — 3.7s
Black `#1a1b1e`. Three hazard-yellow lines slam in one at a time, big condensed caps, centered: 「8 名玩家。」(in), 「7 種線索。」(in), 「1 個最佳解。」(final, largest). A thin hazard-stripe rule under the last line.
Sequential/interaction: yes — three lines arrive one by one, each held legibly (~0.8s+); final line lands on the 3.70s strong cue.
Audio intent: each line a quiet, confident tick; the third lands with a soft dry impact.
Audio-coupled idea: beat-aligned reveal (1.60s, 2.65s, 3.70s).
Music: warm bed enters.
Transition mood: hard cut → Scene 2

### Scene 2 — The noise (Discord clue channel) — 4.3s
Recreate the Discord clue channel: dark chat, avatar + username rows. Eight messages post one by one with the verbatim cryptic clue strings in monospace: `123567 123567`, `1234 134`, `11345666 0`, `12222337 1`, `122334456 5`, `123 123`, `123 0`, `123 123`. Caption overlay bottom: 「人類算不出來。」
Sequential/interaction: yes — 8 message rows arrive one by one with soft pops; snap to beat grid 5.28–7.91, hold the full stack after.
Audio intent: light, accumulating message pops — the sound of a channel filling with noise.
Audio-coupled idea: sequential card/message pops.
Music: steady bed.
Transition mood: clean wipe → Scene 3

### Scene 3 — The command + over-engineering reveal — 4.95s
Discord composer: a `/exchange` slash command chip types in (blurple), Enter. Bot reply bubble: 「開始計算！」 then a live 「正在輸入…」 typing indicator with three breathing dots. Then a deadpan stat slab fades over: **「1,855 種組合」** with subline **「用 C++ 暴力解」**. Lands the compute punch on the 12.65s strong cue.
Sequential/interaction: yes — simulate typing `/exchange` (key ticks), then dots animate, then stat reveal.
Audio intent: crisp key ticks for the command; a low processing tick under the dots; a single dry hit on the 1,855 reveal.
Audio-coupled idea: typed command + count tick.
Music: bed holds, slight lift into the reveal.
Transition mood: hard cut → Scene 4

### Scene 4 — The result (optimal trade plan) — 4.6s
A Discord code block prints the result, monospace. Header row `號碼   1 2 3 4 5 6 7` hint, then trade arrows arrive one per beat: `Player2  →  Player1 : 4`, `Player3  →  Player1 : 7`, `Player5  →  Player2 : 1`, `Player4  →  Player6 : 3`. Then 「計算完成！」 flips to success-green with a check.
Sequential/interaction: yes — 4 arrow rows arrive one by one (card-place each), held legibly; success flips last.
Audio intent: each arrow a light placed-card tick; the completion is one clean positive accent.
Audio-coupled idea: card-place per row + success accent.
Music: bed steady, building toward outro.
Transition mood: soft cut → Scene 5

### Scene 5 — Logo / punchline — 1.95s
Black. Hazard-yellow lockup 「明日方舟線索交換器」, tagline 「把每日換線索，變成一行指令。」, tiny dry `ver 1.2` bottom-right. Hazard-stripe accent. Music fades.
Sequential/interaction: none.
Audio intent: logo settles on a strong cue (~17.91s); music fades out clean.
Audio-coupled idea: final logo accent.
Music: gentle fade-out.
Transition mood: end.

**Music mood for this video:** parody (warm, business-y, restrained)
**Audio summary:** A warm corporate bed carries straight-faced through three declarative title beats, a channel filling with noise, a typed command and a dry "1,855 combinations / C++" flex, then crisp placed-card ticks as the optimal trades print and one clean success accent — competence as comedy, fading out under the logo.
