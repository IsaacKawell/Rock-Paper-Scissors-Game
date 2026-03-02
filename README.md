# Rock-Paper-Scissors-Game
Console-based Rock-Paper-Scissors game (COP1334 Midterm Project) using C++17 and OOP.
# Rock-Paper-Scissors Game

Console-based Rock-Paper-Scissors game implemented in C++ using an object-oriented programming structure (OOP).

---

## How to Compile and Run

This project was developed using C++17 and compiled with g++ (MinGW/MSYS2 on Windows 11).

### Compile

```bash
g++ -std=c++17 rps_main.cpp rps_game.cpp -o rps.exe
```

### Run

```bash
.\rps.exe
```

---

## Project Overview

This program uses an object-oriented design to organize the game logic in a clear and modular way. Declarations and implementations are separated using header (.h) and source (.cpp) files.

The Move and Outcome enumerations define possible moves and round results. A base abstract class Player defines the interface for obtaining a move. The HumanPlayer class manages validated user input, while the ComputerPlayer class generates random moves using the <random> library.

The Game class controls:
- Menu system
- Best-of-N series logic
- Endless mode
- Round resolution
- Score tracking
- Cumulative statistics

The program ensures proper input validation, fair random move generation, automatic termination when a player reaches ceil(N/2) wins, and correct display of running scores.

---

## Features

- Best-of-N series (N must be odd)
- Endless mode
- Case-insensitive input validation (R/P/S or full words)
- Random computer move generation using <random>
- Running scoreboard
- Early quit option (Q)
- Automatic series termination
- Cumulative statistics
- Clean OOP structure with header/source separation
