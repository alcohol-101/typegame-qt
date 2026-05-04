命令行测试两个游戏：
 build\bin\Release\typegame.exe apple --test --input test_config_apple.json --output test_result_apple.json
 build\bin\Release\typegame.exe space --test --input test_config_space.json --output test_result_space.json

 单元测试：运行run_tests.bat文件

 res.qrc=res_main.qrc+res_game.qrc.
 单元测试使用res.qrc文件;启动程序使用res_main.qrc文件，游戏逻辑使用res_game.qrc+word.txt编译成的res_game.rcc文件。

 