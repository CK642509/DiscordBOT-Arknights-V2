# DiscordBOT-Arknights-V2

這是基於 [DiscordBOT-Arknights](https://github.com/CK642509/DiscordBOT-Arknights) 所重構的。

此 Discord BOT 使用 discord.py 開發，核心演算的演算法使用 C++ 撰寫 (由朋友 Yao 開發)。

## 簡介

這是一個可以幫助明日方舟 (Arknights) 玩家的 Discord BOT，可以計算玩家之間線索交換的最佳策略，最大化地獲得遊戲獎勵。

### 動機

明日方舟遊戲中有一類稱為「線索」的道具，一共有編號 1 到 7 號七種不同的線索，玩家每天可以隨機蒐集到數個線索。當玩家蒐集完整一套 (1 到 7 號) 線索之後，可以獲得遊戲獎勵。此外，玩家也可以贈送線索給其他玩家，贈送和接收線索同樣也可以獲得遊戲獎勵，但接收來的線索不能再繼續轉送。不過，接收線索的獎勵有每日上限，一天最多拿三次。

![](https://firebasestorage.googleapis.com/v0/b/images-7e754.appspot.com/o/ithome_2024%2F27_clue.png?alt=media&token=dccc518d-0bba-4f3b-bc2b-3f1b6c1dc6ee)

朋友 Yao 用 C++ 寫了一個計算最佳策略的程式，只要輸入每位玩家所擁有的線索資訊，就可以進行計算。

但是「蒐集線索 --> 執行程式 --> 公告計算結果」這個過程不是很方便，於是希望透過 Discord BOT，讓大家只要每天在 Discord 特定頻道留下當天的線索資訊，待大家都留言後，再開始計算，就可以自動取得最佳化的計算結果並公告在 Discord 上。

## Discord BOT 使用方法

主要的使用步驟如下：

1. 所有玩家都在線索頻道留下線索資訊
2. 在任意頻道呼叫 `/exchange` 這個 slash command，開始計算
3. 計算完畢後，就會在線索頻道看到計算結果

## Discord BOT 功能

### 1. 線索管理
- 在指定頻道傳送訊息時，自動蒐集玩家的線索資訊。
- 編輯、刪除訊息時，可以自動更新已蒐集的線索資訊。
- 可以為他人 (其他玩家) 設定線索資訊。
- 可以根據一日內歷史訊息內容，自動蒐集玩家的線索資訊。

### 2. 最佳化計算
- 可以透過指令要求開始計算。
- 要求開始計算時，如果當天還有人還沒更新線索資訊，會談出提示訊息詢問是否確定要開始計算。
- 計算期間會顯示「正在輸入...」的狀態提示。
- 計算期間不會阻塞 Discord BOT 其他功能。
- 計算完畢後會自動公告結果，並將結果儲存起來 (只保留最新一次的計算結果)。
- 可以隨時呼叫 Discord BOT 查看最新的計算結果。

## 如何架設
### 1. 安裝
```
pip install -r requirements.txt
```

### 2. 更新設定
請參考 `example.toml`，建立一個 `config.toml`，並填入設定。

1. `discord`
   - `guild_id`：Discord 伺服器 ID
   - `clue_channel_id`：蒐集線索資訊的頻道 ID
   - `info_channel_id`：紀錄線索資訊的頻道 ID (可用來確認各位玩家的線索資訊與更新日期)

2. `discord.bot`
   - `token`：Discord BOT 的 token

3. `users`
   - `id`：Discord 成員的「使用者 ID」
   - `name`：顯示的玩家名稱

### 3. 啟動
```
python main.py
```

## Docker 執行方式

Multi-stage Dockerfile 會在建置階段於 Linux 環境自動編譯 `utils/exchange/main`，因此容器中無須放置 `main.exe`。

1. 建立 Docker image
   ```bash
   docker build -t arknights-bot:0.1.0 .
   ```

2. 啟動容器（以 volume 方式掛載含機敏資訊的設定檔）
   ```bash
   docker run --rm -v path/to/config.toml:/app/config.toml:ro --name arknights-bot arknights-bot:0.1.0
   ```

## 測試線索交換演算法

1. 建立 Docker image
   ```
   docker build -f Dockerfile.exchange -t exchange-main-test .
   ```

2. 啟動容器
   ```
   docker run --rm -it --entrypoint /bin/bash exchange-main-test
   ./main
   ```
   