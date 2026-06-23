# Sky Hoopers 🚀

Sky Hoopers is a modern, dynamic 2D platformer inspired by the classic *Doodle Jump*. Built entirely with **C++** and the **SFML** library, this game introduces unique mechanics such as multi-directional gravity, interactive portals, and powerful character buffs to take the endless jumping experience to the next level.

## ✨ Features

- **Two-Way Responsive Scrolling:** The camera and gameplay dynamically adjust, allowing for both vertical and horizontal traversal depending on the current gravity state.
- **Portals:** Special portals that transport the player and alter the direction of gravity and scrolling.
- **Power-ups & Buffs:**
  - 🚀 **Rocket:** Propels the player forward/upward at high speed, making them temporarily invincible.
  - 🛡️ **Shield:** Protects the player from hazards and prevents falling.
  - ⭐ **2x Multiplier:** Doubles the score gained from moving and jumping.
- **Dynamic UI System:** Features an interactive Main Menu, Guide screens, and "Click to Restart" logic.
- **Rich Audio:** Includes persistent Background Music (BGM) and responsive Sound Effects (SFX) for power-ups, portals, and game over states.

## 🛠️ Prerequisites

To build and run this game, you will need the following installed on your system (Linux/WSL, Windows, or macOS):

- A **C++17** compatible compiler (GCC, Clang, or MSVC)
- **CMake** (version 3.10 or higher)
- **SFML** (Simple and Fast Multimedia Library - version 2.5 or higher)

*Note for Ubuntu/WSL users:*
```bash
sudo apt update
sudo apt install cmake libsfml-dev build-essential
```

## 🚀 How to Build & Run

This project uses **CMake** for its build system.

1. **Clone the repository:**
   ```bash
   git clone https://github.com/hideaki7/Game-Proglan-Sky-Hoopers.git
   cd Game-Proglan-Sky-Hoopers
   ```

2. **Create a build directory:**
   ```bash
   mkdir build && cd build
   ```

3. **Configure and compile:**
   ```bash
   cmake ..
   make
   ```

4. **Run the game:**
   ```bash
   ./doodlejump
   ```
   *(Note: Ensure your assets folder is located in the correct working directory as expected by the executable).*

## 🎮 Controls

- **[A]** / **[D]** or **[Left Arrow]** / **[Right Arrow]**: Move the character Left and Right.
- **[Mouse Click]**: Interact with menu buttons (Start, Guide, Quit, Restart).
- Navigate through portals to experience shifting gravity directions!

## 📂 Project Structure

- `src/` - Contains all C++ source code and header files.
  - `entities/` - Player, Platforms, Portals, and Buff logic.
  - `state/` - Game state management (Menu, Guide, Gameplay).
- `assets/` - Game assets including images, fonts, and audio files.

---
*Created as part of an Advanced Programming (Proglan) Project.*
