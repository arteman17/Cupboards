# 🎮 Cupboards - Intelligent Puzzle Game
<img width="799" height="641" alt="image" src="https://github.com/user-attachments/assets/c9af439e-a403-4a62-8b3a-41db5e1a6475" />

**Cupboards** is an engaging puzzle game where players strategically move colored chips across a network of connected points to achieve the target configuration.

## 🎯 Features

- **Strategic Gameplay**: Move chips through interconnected points while avoiding obstacles
- **Multiple Levels**: Progressive difficulty with different board configurations
- **Smart Pathfinding**: Advanced calculating available moves considering blockages
- **Visual Feedback**: Highlighted available moves and selected chips
- **Mini-Map**: Always-visible target configuration display
- **Smooth Animations**: Polished movement and visual effects
- **Mouse Controls**: Intuitive point-and-click interface

## 🛠️ Technical Implementation
# Architecture
 - **C++17** with **Cocos2d-x** game engine
 - **Object-Oriented Design** with proper separation of concerns
 - **Model-View-Controller** pattern implementation
 - **Cross-platform** compatibility (Windows focus)
# Key Components
 - **Board Management:** Dynamic level loading and chip placement
 - **PathFinder:** BFS-based pathfinding with obstacle avoidance
 - **Game State:** Progress tracking and save/load functionality
 - **UI System:** Custom message boxes and interactive elements

## 🎮 How to Play
 1. Select a chip by clicking on it
 2. Available moves will be highlighted in green
 3. Click on destination point to move the chip
 4. Complete the level by matching the target configuration
 5. Use the mini-map in the bottom-right for reference
