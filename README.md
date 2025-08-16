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

<img width="775" height="994" alt="Screenshot 2025-05-08 202605" src="https://github.com/user-attachments/assets/b97a5d9c-c4b7-4011-8c60-a98e947038d9" />

<img width="771" height="999" alt="Screenshot 2025-05-08 202621" src="https://github.com/user-attachments/assets/1ffe459e-6582-4e1f-849b-abc0473e974e" />

<img width="771" height="1001" alt="Screenshot 2025-05-08 202633" src="https://github.com/user-attachments/assets/3405fdd0-70e4-4f1a-bcfb-70f83a21f487" />

<img width="768" height="1001" alt="Screenshot 2025-05-08 202642" src="https://github.com/user-attachments/assets/64743ded-7bc3-482a-be1d-8ae88f44bc1d" />

<img width="769" height="991" alt="Screenshot 2025-05-08 202653" src="https://github.com/user-attachments/assets/90ecc5b6-9f43-4470-8757-51b0f1277ef9" />

<img width="772" height="1000" alt="Screenshot 2025-05-08 202703" src="https://github.com/user-attachments/assets/834078f9-ac6e-4ab0-b480-45dd84d55c74" />

<img width="775" height="999" alt="Screenshot 2025-05-08 202712" src="https://github.com/user-attachments/assets/372d9df2-0f30-43ca-bcc7-4e885793bfe9" />

<img width="772" height="1003" alt="Screenshot 2025-05-08 202723" src="https://github.com/user-attachments/assets/24c4c74a-3d6a-4904-96a0-56e9cdcf2fc9" />

<img width="773" height="1000" alt="Screenshot 2025-05-08 202735" src="https://github.com/user-attachments/assets/9a482b35-4cd2-4b4a-9eaf-0c5abbd953af" />

<img width="808" height="1054" alt="Screenshot 2025-05-08 203922" src="https://github.com/user-attachments/assets/16418f0a-e0be-4d48-ae75-514d8eba94e3" />

<img width="810" height="1055" alt="Screenshot 2025-05-08 203936" src="https://github.com/user-attachments/assets/08bea76e-9324-432f-ad41-cf022530a084" />





















































































