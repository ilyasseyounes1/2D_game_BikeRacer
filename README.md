[Project_bicke.pdf](https://github.com/user-attachments/files/21816881/Project_bicke.pdf)
# 🚴‍♂️ 2D Bike Racer Game

A simple yet engaging **2D arcade bike racing game** built in **C++** with the **SFML** library.  
Your mission: dodge incoming obstacles, survive as long as possible, and rack up game time!

---

## 📜 Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Gameplay](#gameplay)
- [Class Architecture](#class-architecture)
- [Development Environment](#development-environment)
- [Screenshots](#screenshots)
- [Future Improvements](#future-improvements)
- [Authors](#authors)

---

## 🏁 Introduction
**Bike Racer** challenges the player to control a motorcycle and avoid an endless stream of obstacles on a scrolling road.  
Core functionality includes:
- Game state management (Menu, Playing, Paused, About)
- Smooth sprite‑sheet animations for the bike
- Collision detection and obstacle generation
- Keyboard controls for responsive movement

---

## ✨ Features
- **Graphical UI** built with SFML
- **Multiple game states**: Menu, Play, Pause, About
- **Random obstacle generation** with different types
- **Sprite sheet animation** for smooth bike movement
- **Collision detection** ending the game on impact
- **Asset management** with error handling

---

## 🎮 Gameplay
- **Controls**:
  - ⬅️/➡️ – Move bike left/right
  - `ESC` – Pause / Resume
- **Objective**: Survive for as long as possible by dodging obstacles.
- **Obstacles**:
  - Cars
  - Construction blocks
  - Road barriers
  - Parked bikes
- **Game Flow**:
  1. Start from the main menu.
  2. Enter gameplay with continuous obstacle generation.
  3. Collision returns you to the main menu.

---

## 🛠 Class Architecture

### `Game` Class
- Central controller: manages window, assets, game states, updates, rendering, and collisions.
- Holds:
  - `sf::RenderWindow` for game display
  - Sprites, textures, fonts
  - Vectors for obstacles and road segments
  - Game timing and UI elements
- Key methods: `run()`, `handleInput()`, `update()`, `render()`, `resetGame()`, `addRandomObstacle()`

### `Obstacle` Base Class & Derived Types
- Base for all obstacle types: handles positioning, scaling, speed, collision box.
- Derived classes:
  - `CarObstacle`
  - `Car2Obstacle`
  - `ConstructionObstacle`
  - `RoadBarrierObstacle`
  - `ParkedBikeObstacle`

---

## 💻 Development Environment
- **Language**: C++17+
- **Library**: SFML (graphics, events, timing)
- **IDE**: VS Code / Visual Studio
- **Assets**: Stored in `/assets` (textures, sprite sheets, fonts)
- **Build**: GCC/Clang/MSVC with SFML linked

---

## 🖼 Screenshots

<img width="808" height="1054" alt="Screenshot 2025-05-08 203922" src="https://github.com/user-attachments/assets/5c152f62-b5c9-4ddb-bc38-0efb9342d635" />

