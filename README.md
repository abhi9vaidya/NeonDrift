# 🏎️ Neon Drift

A futuristic 2D arcade racing game built with C++ and SFML. Features neon visuals, drift mechanics, and combo-based scoring.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=flat-square&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/SFML-2.5+-green?style=flat-square)
![CMake](https://img.shields.io/badge/CMake-3.16+-orange?style=flat-square&logo=cmake)
![License](https://img.shields.io/badge/License-MIT-purple?style=flat-square)

## 🎮 Game Concept

Neon Drift is a top-down 2D drifting game where you control a futuristic vehicle on glowing tracks. Master the art of drifting, build combos, and survive as the difficulty increases!

### Features
- 🚗 Smooth physics-based vehicle controls
- 💨 Drift mechanics with combo scoring
- ✨ Stunning neon visual effects & particle systems
- 🎵 Synthwave audio design
- 📈 Progressive difficulty system

## 🎯 Controls

| Key | Action |
|-----|--------|
| W / ↑ | Accelerate |
| S / ↓ | Brake |
| A / ← | Turn Left |
| D / → | Turn Right |
| Space | Drift |
| Esc | Pause |

## 🛠️ Building

### Prerequisites
- C++17 compatible compiler
- CMake 3.16+
- SFML 2.5+

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/abhi9vaidya/NeonDrift.git
cd NeonDrift

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run the game
./NeonDrift
```

## 📁 Project Structure

```
NeonDrift/
├── src/                 # Source files
│   ├── core/           # Game loop, states, managers
│   ├── entities/       # Player, obstacles, track
│   ├── graphics/       # Rendering, particles, effects
│   ├── audio/          # Audio management
│   └── ui/             # HUD and menus
├── include/            # Header files
├── assets/             # Game assets
│   ├── fonts/
│   ├── sounds/
│   └── music/
├── CMakeLists.txt
└── README.md
```

## 🎨 Tech Stack

- **Language:** C++17
- **Graphics/Window:** SFML 2.5
- **Audio:** SFML Audio
- **Build System:** CMake

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

---

