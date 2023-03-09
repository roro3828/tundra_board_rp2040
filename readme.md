公式 https://github.com/tundra-labs/rp2040_examples

# 使い方
## ドライバの登録
コマンドプロンプトまたはPowerShellからvrpathreg.exe(SteamVR/bin/win64/vrpathreg.exe)を実行
```cmd
vrpathreg.exe joystick\driver\joystick_controller
```
引数を何も指定しないと登録されているドライバを見ることが出来る

## lighthouse_console.exeからトラッカーの設定ファイルを書き換え
```
downloadconfig
```
を実行してデフォルトの設定ファイルをダウンロード  
このファイルは元に戻す際に必要になるためコピーを作りそっちを変更  
```
    "firmware_config" : {
      "mapped_input" : true,
      "haptic_driver" : "mapped_input",
      "remap_channel_13" : 6,
      "remap_channel_14" : 7,
      "remap_channel_15" : 8,
```
input_profileを変更
```
"input_profile" : "{joystick_controller}/input/joystick_controller_profile.json",
```
変更したらlighthouse_consoleで書き込む
```
uploadconfig joystick\device_json\joystick_controller.json
```
## ボードにプログラムを書き込む