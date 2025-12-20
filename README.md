# sresult
C++17でstd::expectedもどきを作ってrailwayを感じてみる

## ビルド方法

cmakeを使います。

### VS2022などマルチコンフィグレーションの場合

VS2022のシェル実行環境(典型的には "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" )から

```shell-session
C:\Users\user\sresult>cmake -B 任意のビルド結果格納用ディレクトリ
...
C:\Users\user\sresult>cmake --build 任意のビルド結果格納用ディレクトリ --verbose --config Release
...
```

### make(MSYS2やLinux)などシングルコンフィグレーションの場合

```shell-session
$ cmake -B 任意のビルド結果格納用ディレクトリ -DCMAKE_BUILD_TYPE=Release
...
$ cmake --build 任意のビルド結果格納用ディレクトリ --verbose
...
```

### C++17/C++23切り替え用のオプション

最初のcmakeの再に`-DUSE_CPP23=on`とすればC++23、それ以外はC++17が使用されます。
この設定はビルド結果格納用ディレクトリにCMakeCache.txtに残るので注意してください(未指定に戻す場合、このファイルをいちいち削除する必要があります)。
