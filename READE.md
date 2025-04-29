# 🚴 Bike Race Game

![Game Screenshot](2D_gmae_v8/assets/menu_bg.png) 
A fast-paced obstacle dodging game built with SFML (C++). Navigate through traffic, avoid hazards, and boost your way to high scores!

## ✨ Features
- **Smooth bike physics** with leaning animations
- **5+ obstacle types** (cars, barriers, puddles, etc.)
- **Progressive difficulty** - gets faster as you play
- **Boost mechanic** for temporary speed bursts
- **Crisp pixel art** animations
- **Responsive controls** (keyboard/mouse)

## 📥 Installation
1. **Requirements**:
   - SFML 2.5+ 
   - C++17 compiler

2. **Build & Run**:
   ```bash
   git clone https://github.com/ilyasseyounes1/2D_game_BikeRacer.git
   cd bike-race
   mkdir build && cd build
   cmake ..
   make
   g++ main.cpp -o BikeRace -lsfml-graphics -lsfml-window -lsfml-systemC
   ./BikeRace