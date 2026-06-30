## Context

ESP32 + TFT 的开发迭代慢：改代码 → 编译 → 烧录 → 看效果需要数分钟。需要一个桌面模拟器，在电脑上实时预览宠物行为和视觉效果。现有 `tests/helpers/mock_esp32.py` 已包含完整游戏逻辑的 Python 实现，可直接复用。

## Goals / Non-Goals

**Goals:**
- 在 macOS 上双击或命令行运行，零额外安装（tkinter + PIL 均自带或 pip install）
- 320×240 Canvas 渲染精灵、背景、状态栏
- 3x 缩放显示（960×720 窗口）
- 键盘驱动交互
- 实时状态显示

**Non-Goals:**
- 真实物理按钮模拟
- 音效
- 多宠物切换 UI
- 存档导出

## Decisions

### D1: tkinter over Pygame

macOS 自带 tkinter，零安装。Pygame 需要 `pip install pygame`。对 320×240 @ 30fps 的渲染，tkinter Canvas + PIL ImageTk 完全够用。

### D2: 直接解析 .h 精灵文件

不做中间 JSON 格式。Python 用正则提取 `const uint16_t xxx[] PROGMEM = {...}` 数组。简单、直接、无额外构建步骤。

### D3: MockESP32 + PetAI 双模块

游戏逻辑直接用 `tests/helpers/mock_esp32.py` 的 `MockESP32`。AI 状态机单独一个轻量 Python 类，与 C++ `pet_ai.h/cpp` 行为一致。

### D4: 事件循环用 tkinter after()

```python
def tick():
    game.update()
    ai.update()
    render_frame()
    root.after(33, tick)  # ~30fps
```

不用独立线程，避免线程安全问题。

## Files

| 文件 | 说明 |
|------|------|
| `simulator.py` | 主程序，约 400 行 |
| `tests/helpers/mock_esp32.py` | 复用，游戏逻辑 |
| `src/pet/sprite_test.h` | 读取，精灵数据 |

## Risks

- PIL/Pillow 可能需要 `pip install Pillow`（macOS tkinter 不带 PIL）。解决方案：`pip install Pillow` 一次。
- RGB565 解析可能有字节序问题。解决方案：写单元测试验证解析结果。
