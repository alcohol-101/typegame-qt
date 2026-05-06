# TypeGame — 打字游戏

基于 Qt5 的 C++ 打字练习游戏，包含「拯救苹果」和「太空大战」两种玩法。

---

## 构建 & 运行

```bash
# Windows（需要 vcpkg + Qt5）
build_win.bat

# 产物
build\bin\Release\typegame.exe
```

### 命令行测试

```
build\bin\Release\typegame.exe apple --test --input test_config_apple.json --output test_result_apple.json
build\bin\Release\typegame.exe space --test --input test_config_space.json --output test_result_space.json
```

### 单元测试

运行 `run_tests.bat`（8 个 Qt Test 用例）。

---

## 架构设计

```
┌────────────┐
│  main.cpp  │── GameTestRunner ──→ CLI测试模式（--test）
└─────┬──────┘
      │
┌─────▼──────┐
│ MainWindow │  无边框主窗口，自定义标题栏，拖拽移动
└─────┬──────┘
      │ 点击游戏图标
      ├──→ AppleGameWidget   拯救苹果（字母掉落 + 按键消除）
      ├──→ SpaceWarWidget    太空大战（敌机字母 + 子弹追踪）
      ├──→ (预留) 生死时速 / 鼠的故事 / 激流勇进
      │
      ▼ typegame_lib（静态库）
```

### 模块分层

| 层 | 职责 | 文件 |
|---|---|---|
| **入口** | 模式分发（GUI / CLI 测试） | `main.cpp` |
| **窗口** | 无边框主窗口 + 游戏选择 | `mainwindow.*` |
| **游戏** | 拯救苹果核心逻辑 | `applegamewidget.*` |
| | 太空大战核心逻辑 | `spacewarwidget.*` |
| **UI 组件** | 图文按钮（hover 变色） | `buttonwithtext.*` |
| | 三态按钮（Normal/Hover/Press） | `tristatebutton.*` |
| | 设置弹窗 / 退出确认 / 高分榜 / 昵称输入 | `settingsdialog.*` `exitconfirmdialog.*` `highscoredialog.*` `nameinputdialog.*` |
| **测试** | CLI 集成测试框架（JSON 驱动） | `gametestrunner.*` |
| | Qt Test 单元测试（8 个） | `test/tst_*.cpp` |
| **资源** | 运行时动态注册 .rcc 资源文件 | `respath.h` |

### 资源管理策略

- `res_main.qrc` — 主窗口资源（编译进 exe）
- `res_game.qrc` + `data/word.txt` → **编译为 `res_game.rcc`**（二进制资源文件）
- 启动游戏时通过 `QResource::registerResource()` 动态加载 .rcc
- **好处**：减小主程序体积，游戏资源按需加载
- `res.qrc = res_main.qrc + res_game.qrc` 供单元测试链接使用

### 测试架构

**双层测试体系**：

1. **CLI 集成测试**（`GameTestRunner`）：模拟键盘输入，JSON 配置驱动，支持 `AllCorrect` / `AllWrong` / `WithErrors` 三种模式，输出结构化 JSON 报告
2. **Qt Test 单元测试**（8 个）：覆盖 ButtonWithText、TriStateButton、NameInputDialog、HighScoreDialog、ExitConfirmDialog、AppleGameWidget、SpaceWarWidget、MainWindow

