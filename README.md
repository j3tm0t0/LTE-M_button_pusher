# 公式ワークブック SORACOM実装ガイド 第三章
## SORACOM LTE-M Button Plus を自動的に押すためのサンプルプログラム


![M5Stick-Cとリレーユニット](img/m5stickc_relay.jpg)

## 使い方
- [M5stick-C 用のスケッチ](auto-push/auto-push.ino) を Arduino IDE などで M5Stick-C に書き込みます



# Appendix
HAT 用端子に下記のような回路を接続すると、より小型の回路でスイッチングできます。

```
                  TLP222AF
+----------+      +---+-+
|       IN <------|3  |2|-----+
|       IN <------|4  |1|-+   |
|          |      +---+-+ |   |
|          |              R   |  R = 200 ohm
|          |              |   |
|          |   +----------+---++
|   LTE-M  |   |5 3 B G G G 5 G|
|  Button  |   |V V A 0 3 2 V N|
|   Plus   |   |I 3 T   6 6 O D|
|          |   +---------------+
|          |   |               |
|          |   |M 5 S t i c k C|
|          |   |               |
```

## パーツリスト
- フォトカプラ TLP222AF
- カーボン抵抗 1/6W 200Ω