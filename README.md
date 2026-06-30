# 🐉 LED Pet - 虚拟宠物系统

基于第一性原理重新设计的 ESP32 虚拟宠物，将宠物视为情感连接的数字生命体。

## 🎯 项目特点

### 核心理念
- **情感连接**：宠物有需求、有个性、有情感，而非简单的游戏机制
- **成长驱动**：进化由积分控制，反映不同的护理风格
- **家长参与**：通过 Web 界面远程互动和奖励

### 功能特性

| 功能 | 描述 |
|------|------|
| **需求系统** | 4 个独立需求：饥饿、快乐、能量、羁绊 |
| **性格系统** | 6 维度性格：playful, shy, curious, lazy, brave, gentle |
| **分支进化** | 4 种进化路径：Kind, Playful, Wild, Wise |
| **成就系统** | 12 种成就 + 称号系统 |
| **视觉表达** | 需求气泡、表情动画、环境变化 |
| **音频反馈** | 6 种音效：喂食、抚摸、玩耍、进化、错误、成就 |
| **Web UI** | 实时状态显示、远程互动、奖励发送 |

## 🏗️ 架构设计

```
src/
├── core/           # 核心抽象（Need, Personality, Clock, Config）
├── pet/            # 宠物实体（Pet, State, Expression）
├── game/           # 游戏系统（Economy, Evolution, Achievement）
├── display/        # 显示系统（Bubble, ExpressionRenderer）
├── world/          # 环境系统（Environment）
├── audio/          # 音频系统（SoundEngine）
├── web/            # Web 服务（PetWebServer）
├── app/            # 应用层（App）
├── input/          # 输入管理（ButtonManager）
├── persistence/    # 数据持久化（Storage）
└── main_new.cpp    # 主程序入口
```

## 🚀 快速开始

### 环境要求
- ESP32 开发板
- PlatformIO CLI 或 VS Code + PlatformIO 插件
- TFT 显示屏（320x240）
- 3 个按钮
- 蜂鸣器

### 编译和烧录

```bash
# 安装依赖
pio lib install

# 编译固件
pio run

# 烧录到 ESP32
pio run --target upload

# 监视串口
pio run --target monitor
```

### 运行测试

```bash
# Python 测试（游戏逻辑）
python3 -m pytest tests/test_game_logic.py tests/test_pet_ai.py -v
```

## 🎮 按钮操作

| 按钮 | 短按 | 长按 |
|------|------|------|
| A | 喂食 | 抚摸 |
| B | 玩耍 | 商店 |
| C | 休息 | 设置 |

## 🌟 进化系统

### 进化阶段
```
Egg → Baby → Teen → Adult → Final
```

### 进化路径（Teen 阶段分支）
| 路径 | 条件 | 最终形态 |
|------|------|----------|
| Kind | 高羁绊 + 均衡护理 | Guardian Dragon |
| Playful | 高快乐 + 多玩耍 | Trickster Dragon |
| Wild | 低羁绊 + 高能量 | Wild Dragon |
| Wise | 规律喂食 + 稳定护理 | Wise Dragon |

### 进化阈值
- Egg → Baby：100 喂食值 + 进化石
- Baby → Teen：200 喂食值 + 进化石
- Teen → Adult：350 喂食值 + 进化石
- Adult → Final：500 喂食值 + 进化石

## 🍎 食物系统

| 食物 | 价格 | 喂食值 |
|------|------|--------|
| 苹果 | 3 积分 | +10 |
| 鸡腿 | 5 积分 | +20 |
| 蛋糕 | 8 积分 | +30 |
| 星星 | 12 积分 | +50 |
| 进化石 | 20 积分 | 触发进化 |

## 🏆 成就系统

| 成就 | 条件 | 奖励 |
|------|------|------|
| 小饲养员 | 连续 3 天喂食 | +20 积分 |
| 忠实守护者 | 连续 7 天喂食 | +50 积分 |
| 喂食大师 | 连续 14 天喂食 | +100 积分 |
| 永恒守护者 | 连续 30 天喂食 | +200 积分 |
| 朋友 | 羁绊值达到 50 | 称号 |
| 挚友 | 羁绊值达到 80 | 称号 |
| 灵魂伙伴 | 羁绊值达到 100 | 称号 |
| 护理大师 | 所有需求 ≥ 80 持续 24 小时 | +100 积分 |

## 🌐 Web API

### 端点
| 方法 | 路径 | 描述 |
|------|------|------|
| GET | /api/status | 获取宠物状态 |
| POST | /api/feed | 喂食 |
| POST | /api/pet | 抚摸 |
| POST | /api/play | 玩耍 |
| POST | /api/reward | 发送奖励（需 PIN） |

### 状态响应示例
```json
{
  "petState": 0,
  "hunger": 85,
  "happiness": 70,
  "energy": 90,
  "bond": 60,
  "points": 150,
  "evolution": 2,
  "path": "Kind"
}
```

## 📁 文件结构

```
led-pet/
├── src/                    # 源代码
│   ├── core/              # 核心系统
│   ├── pet/               # 宠物实体
│   ├── game/              # 游戏逻辑
│   ├── display/           # 显示系统
│   ├── world/             # 环境系统
│   ├── audio/             # 音频系统
│   ├── web/               # Web 服务
│   ├── app/               # 应用层
│   ├── input/             # 输入管理
│   ├── persistence/       # 数据持久化
│   └── main_new.cpp       # 主程序
├── tests/                 # 测试文件
├── data/                  # Web UI 静态文件
├── .claude/               # Claude 配置
├── platformio.ini         # PlatformIO 配置
├── README.md              # 项目文档
├── IMPLEMENTATION_SUMMARY.md  # 实施总结
└── CODE_REVIEW_FIXES.md   # 代码审查修复
```

## 🧪 测试

### 测试覆盖
- **单元测试**：Need, Personality, Clock, Pet, State, Expression
- **集成测试**：游戏逻辑、宠物 AI
- **测试结果**：36/36 通过

### 运行测试
```bash
# Python 测试
python3 -m pytest tests/ -v

# 特定测试
python3 -m pytest tests/test_game_logic.py -v
python3 -m pytest tests/test_pet_ai.py -v
```

## 🔧 配置

### 按钮引脚
```cpp
#define BTN_A_PIN 32
#define BTN_B_PIN 33
#define BTN_C_PIN 34
```

### 需求衰减率
```cpp
饥饿：1 点/10 分钟
快乐：1 点/30 分钟
能量：1 点/20 分钟
羁绊：-1 点/2 小时
```

## 📝 开发日志

### v1.0.0 (2026-06-30)
- ✅ 完成核心需求系统
- ✅ 实现性格系统
- ✅ 实现分支进化系统
- ✅ 实现成就系统
- ✅ 实现 Web UI
- ✅ 完成代码审查和修复

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📄 许可证

MIT License

## 🔗 相关链接

- [PlatformIO 文档](https://docs.platformio.org/)
- [ESP32 Arduino 核心](https://github.com/espressif/arduino-esp32)
- [TFT_eSPI 库](https://github.com/Bodmer/TFT_eSPI)
