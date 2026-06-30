# CLAUDE.md

This file provides guidance to Claude Code when working with code in this repository.

## Project Overview

LED Pet is an ESP32-based virtual pet system designed with first principles thinking. The pet is a digital companion with needs, personality, and emotions - not just a game mechanic.

**Key Design Principles:**
- Emotional connection over game mechanics
- Growth driven by points (parent-controlled)
- Branch evolution reflecting care style

## Architecture

```
src/
├── core/           # Core abstractions (Need, Personality, Clock, Config)
├── pet/            # Pet entity (Pet, State, Expression)
├── game/           # Game systems (Economy, Evolution, Achievement)
├── display/        # Display system (Bubble, ExpressionRenderer)
├── world/          # Environment system (Environment)
├── audio/          # Audio system (SoundEngine)
├── web/            # Web server (PetWebServer)
├── app/            # Application layer (App)
├── input/          # Input management (ButtonManager)
├── persistence/    # Data persistence (Storage)
└── main_new.cpp    # Main entry point
```

## Key Systems

### Need System (src/core/need.h)
- 4 needs: Hunger, Happiness, Energy, Bond
- Each has independent decay rate
- `drain()` for consumption, `recover()` for restoration

### Personality System (src/core/personality.h)
- 6 traits: playful, shy, curious, lazy, brave, gentle
- Develops through interactions

### Evolution System (src/game/evolution.h)
- 5 stages: Egg → Baby → Teen → Adult → Final
- 4 branch paths: Kind, Playful, Wild, Wise
- Requires feed count threshold + evolution stone

### Achievement System (src/game/achievement.h)
- 12 achievements with titles
- Categories: consecutive feeding, evolution milestones, emotional connection, care master

## Build & Test

```bash
# Build firmware
pio run

# Flash to ESP32
pio run --target upload

# Run Python tests
python3 -m pytest tests/ -v
```

## Button Mapping

| Button | Short Press | Long Press |
|--------|-------------|------------|
| A | Feed | Pet |
| B | Play | Shop |
| C | Rest | Settings |

## Web API

| Method | Path | Description |
|--------|------|-------------|
| GET | /api/status | Get pet status |
| POST | /api/feed | Feed pet |
| POST | /api/pet | Pet the pet |
| POST | /api/play | Play with pet |
| POST | /api/reward | Send reward (requires PIN) |

## Important Constants

- Button pins: `BTN_A_PIN=32`, `BTN_B_PIN=33`, `BTN_C_PIN=34`
- Decay rates: Hunger (10min), Happiness (30min), Energy (20min), Bond (2hr)
- Evolution thresholds: 100/200/350/500 feed count

## Known Issues (from code review)

- OTA uses hardcoded credentials (needs change before deployment)
- No PIN rate limiting (needs implementation)
- Some performance optimizations pending (dirty rectangle tracking)

## Related Files

- `README.md` - User-facing documentation
- `IMPLEMENTATION_SUMMARY.md` - Milestone completion status
- `CODE_REVIEW_FIXES.md` - Code review findings and fixes
- `.claude/prds/led-pet-redesign.prd.md` - Product requirements
