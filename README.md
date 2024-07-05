# 🕹️ Pokemon vs TeamRocket

## 🌟 Overview
This project, developed during my first year of learning C in Prépa, is a tower defense game with a "Pokemon vs TeamRocket" theme. The objective of the game is similar to other tower defense games: you must place Pokemon on different lines to defend against waves of Team Rocket enemies advancing towards the left side of the screen. Each Pokemon can attack and eliminate enemies on its line. Make sure to manage your credits wisely to always have a response to new waves!

## 📜 Game Objective
The goal is to generate defenses (placed at the beginning of the game by the player) using Pokemon as turrets to destroy the advancing Team Rocket enemies. The game ends when either all the enemies are defeated or they reach the left side of the screen. Different waves of enemies can be generated depending on the desired difficulty level.

## 💻 Technical Details

### 🧩 ASCII Version
In the ASCII version, when the program starts, you can observe the wave of enemies you need to confront. When ready, press any key to prepare your defenses. You will then enter the phase where you can place Pokemon. You need to specify the type, line, and position of the Pokemon. Be mindful of your credits! Once ready, the game starts, and you can see if your defenses are strong enough to handle the wave of enemies.

### 🖼️ Graphical Version
In the graphical version, when the program starts, you can observe the game board where the action takes place. You can click on one of the four types of Pokemon displayed at the top left, then place it on a tile. Be careful, as clicking anywhere else starts the wave! Once you decide to take action, the game begins.

### 🛠️ Project Structure
The project is divided into two main parts: the ASCII version and the graphical version. It was crucial to first develop a functional ASCII version as a solid foundation for the graphical version.

### 🔍 Key Components
Structures: Implemented structures for Chips, Virus, and Game.
Virus Management: Functions to handle the characteristics and positioning of viruses.
Memory Management: Utilized mallocs to manage memory efficiently and avoid segmentation faults.
Tourelles (Pokemon): Functions similar to viruses, but adapted for Pokemon.
Damage Handling: Functions to manage attacks and damage between Pokemon and enemies.
Victory Conditions: Functions to determine the end of the game based on victory or defeat.
### 🛠️ Prerequisites
To run this project, you will need:

C Compiler: Ensure you have a C compiler installed (e.g., gcc).
Graphics Library: If you plan to run the graphical version, ensure you have the necessary graphics libraries installed.

## 🚀 Running the Project

### ASCII Version
**Compile the Code:**
```bash
gcc -o pokemon_vs_teamrocket ascii_version.c
```
Run the Game:
```bash
./pokemon_vs_teamrocket
```

### Graphical Version
**Compile the Code:**
```bash
gcc -o pokemon_vs_teamrocket graphical_version.c -lgraphicslib
```
**Run the Game:**
```bash
./pokemon_vs_teamrocket
```

📧 Contact
For any inquiries or further information, please contact:

Lucien Laumont: lucien.laumont@edu.esiee.fr
Joanna Gosse: ???