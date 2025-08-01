# Geometry Wars

A 2D arena shooter game built with C++ and SFML, implementing Entity-Component-System (ECS) architecture. This is a recreation of the classic Geometry Wars game with modern C++ features and clean code design.

## 🎮 Game Overview

Geometry Wars is an intense 2D shooter where you control a geometric shape in an arena filled with enemy shapes. Your goal is to survive as long as possible while destroying enemies and achieving the highest score possible.

### Features

- **Player Movement**: Smooth WASD/Arrow key movement with screen boundary collision
- **Mouse Aiming**: Shoot bullets toward the mouse cursor with left-click
- **Special Ability**: Press 'X' to activate a special ability with cooldown
- **Dynamic Enemies**: Randomly spawning enemies with varying shapes, colors, and speeds
- **Enemy Splitting**: Large enemies split into smaller enemies when destroyed
- **Score System**: Points awarded based on enemy complexity (number of vertices)
- **Visual Effects**: Rotating entities, alpha blending for lifespan effects
- **Pause System**: Press 'P' to pause/unpause the game

## 🏗️ Architecture

This project demonstrates Entity-Component-System (ECS) design patterns:

### Core Components
- **CTransform**: Position, velocity, and rotation
- **CShape**: Visual representation with SFML CircleShape
- **CCollision**: Collision detection radius
- **CScore**: Point value for entities
- **CLifeSpan**: Time-based entity lifecycle
- **CInput**: Player input handling
- **CSpecialAbility**: Special ability cooldown management

### Systems
- **Movement System**: Updates entity positions and velocities
- **Input System**: Handles player input and controls
- **Lifespan System**: Manages entity lifecycle and alpha blending
- **Render System**: Draws all entities with rotation effects
- **Collision System**: Detects and handles entity collisions

## 🚀 Building and Running

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- SFML 2.5+ library
- CMake (optional, for easier build management)

### Dependencies
Install SFML on macOS:
```bash
brew install sfml
```

### Compilation
```bash
g++ -std=c++17 -g -I/opt/homebrew/include -L/opt/homebrew/lib \
    -lsfml-graphics -lsfml-window -lsfml-system \
    main.cpp Game.cpp EntityManager.cpp Entity.cpp Vec2.cpp \
    -o GeometryWars
```

### Running
```bash
./GeometryWars
```

## ⚙️ Configuration

The game is configured through `config.txt` with the following format:

```
Window 1280 720 60 0
Font "/Library/fonts/arial.ttf" 24 255 255 255
Player 32 32 5 5 5 5 255 0 0 4 8
Enemy 32 32 255 255 255 2 3 8 90 180 3 6
Bullet 10 10 8 0 255 0 0 255 0 4 60 15
```

### Configuration Parameters

**Window**: `width height framerate fullscreen(0/1)`
**Font**: `path size red green blue`
**Player**: `shapeRadius collisionRadius speed fillR fillG fillB outlineR outlineG outlineB outlineThickness vertices`
**Enemy**: `shapeRadius collisionRadius outlineR outlineG outlineB outlineThickness minVertices maxVertices minSpeed maxSpeed lifespan spawnInterval`
**Bullet**: `shapeRadius collisionRadius speed fillR fillG fillB outlineR outlineG outlineB outlineThickness vertices lifespan`

## 🎯 Gameplay

### Controls
- **WASD/Arrow Keys**: Move player
- **Left Mouse Button**: Shoot bullets
- **X**: Activate special ability
- **P**: Pause/Unpause game
- **Escape**: Exit game

### Scoring
- Enemies are worth `vertices × 100` points
- Small enemies (from splitting) are worth double points
- Score is displayed in the top-left corner

### Enemy Behavior
- Enemies spawn randomly around screen edges
- They bounce off screen boundaries
- Large enemies split into smaller enemies when destroyed
- Small enemies travel outward at fixed angles based on original vertex count

## 📁 Project Structure

```
GeometryWars/
├── main.cpp              # Entry point
├── Game.h/.cpp           # Main game class and systems
├── Entity.h/.cpp         # Entity class definition
├── EntityManager.h/.cpp  # Entity management and queries
├── Components.h/.cpp     # ECS component definitions
├── Vec2.h/.cpp          # 2D vector mathematics
├── config.txt           # Game configuration
├── Specifications.txt   # Original project requirements
└── README.md           # This file
```

## 🔧 Development

This project was built as an educational exercise in:
- Modern C++ (C++17) features
- Entity-Component-System architecture
- SFML graphics programming
- Game development patterns
- Memory management with smart pointers

### Key Design Decisions
- Uses `std::shared_ptr` for entity ownership
- Implements pure ECS without inheritance
- Configurable game parameters via text file
- Clean separation of concerns between systems

## 🎨 Visual Features

- Smooth entity rotation for dynamic visuals
- Alpha blending for lifespan-based transparency
- Random enemy colors and shapes
- Configurable visual properties for all entities

## 🐛 Known Issues

- Some segmentation faults may occur during intense gameplay
- Performance may degrade with many entities on screen

*Built with ❤️ using C++ and SFML*
