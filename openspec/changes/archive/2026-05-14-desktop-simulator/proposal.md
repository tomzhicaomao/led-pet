## Why

开发和调试 LED Pet 需要在电脑上看到宠物行为和视觉效果。当前 C++ → 编译 → 烧录 → 看效果的循环一次 3-5 分钟，视觉调整极其低效。需要一个本地运行的桌面模拟器，直接使用现有 pytest 的游戏逻辑 mock（`MockESP32`），加上 tkinter 渲染，实现秒级迭代。

## What Changes

- 新建 `simulator.py`：tkinter GUI 程序，模拟 TFT 320×240 显示，内置完整宠物循环
- 移植 `PetAI` 状态机到 Python（与 C++ `pet_ai.h/cpp` 行为一致）
- 从 C 头文件直接解析 RGB565 精灵数组，渲染到 Canvas
- 键盘映射：1/2/3 = 按钮 A/B/C，R = 父母发奖励，F = 强制喂食
- 实时显示状态栏：宠物名、阶段、积分、当前状态

## Capabilities

### New Capabilities

- `desktop-simulator`: tkinter 桌面窗口，Canvas 渲染 TFT 模拟画面，键盘驱动交互，复用 MockESP32 游戏逻辑

### Modified Capabilities

None — 新增工具，不修改现有模块。

## Impact

- **新文件**: `simulator.py` (约 400 行)
- **依赖**: Python tkinter（macOS 自带），PIL/Pillow 用于图像缩放
- **复用**: `tests/helpers/mock_esp32.py` 的游戏逻辑
- **不修改**: 任何现有 C++/Python 源文件
