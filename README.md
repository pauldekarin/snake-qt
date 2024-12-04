# Snake Game (BrickGame v2.0)

## Overview

This project is an implementation of the classic Snake game, developed as part of the School 21 curriculum. The project is divided into two main parts: a library that handles the game logic and a desktop interface built using Qt.

## Part 1: Main Task

### Specifications

- **Language**: C++ (C++17 standard)
- **Structure**: The project consists of two main parts:
  - A library that implements the logic of the Snake game.
  - A desktop interface for the game.

### Requirements

- **Finite State Machine**: The game logic is formalized using a finite state machine (FSM). The FSM specification is provided in `materials/library-specification.md`.
- **Directory Structure**:
  - The game logic library is located in `src/brick_game/snake`.
  - The interface code is located in `src/gui/desktop`.
- **Coding Style**: Follow the Google Style Guide.
- **Namespace**: All classes are implemented within the `s21` namespace.
- **Unit Tests**: The game logic library is covered by unit tests using the GTest library, ensuring at least 80% test coverage, particularly for FSM states and transitions.
- **Build System**: The project is built using a Makefile with standard targets: `all`, `install`, `uninstall`, `clean`, `dvi`, `dist`, `test`. The installation directory can be arbitrary.
- **GUI Library**: The GUI is based on Qt, which provides an API compatible with C++17.
- **MVC Pattern**: The project follows the Model-View-Controller (MVC) pattern:
  - No business logic in the view code.
  - No interface code in the model, presenter, or view model.
  - Controllers are kept lightweight.

### Gameplay Mechanics

- The snake moves automatically, advancing by one block when the game timer expires.
- When the snake eats an "apple", its length increases by one unit.
- The game ends when the snake's length reaches 200 units (win condition) or if it collides with the field boundary or itself (lose condition).
- Players can change the snake's direction using arrow keys, but the snake can only turn left or right relative to its current direction.
- Players can speed up the snake by pressing an action key.
- The initial snake length is four "pixels".
- The playing field is 10 "pixels" wide and 20 "pixels" high.

## Part 2: Bonus - Scoring and Game Record

### Additional Mechanics

- **Scoring**: Players earn one point for each apple eaten.
- **High Score**: The game keeps track of the maximum score, which is stored in a file or an embedded DBMS and persists between game sessions.
- The maximum score is updated if the player exceeds the current high score during gameplay.

## Part 3: Bonus - Level Mechanics

### Level Progression

- Each time a player earns 5 points, the game level increases by one.
- Increasing the level results in a faster snake speed.
- The maximum number of levels is 10.

## Installation

### Prerequisites

- Qt
- GCC
- GTest

### Building the Project

1. Clone the repository:
     ```sh
     git clone https://github.com/pauldekarin/snake_game.git
     cd snake_game
     ```
2. Build the project using the Makefile:
    ```sh
    make all
    ```
    
## Run the tests:
```sh
make test
```

## Install the game:
```sh
make install
```

## Running the Game
To run the game, execute the following command after installation:
```sh
./bin/snake_game
```
## License
This project is licensed under the MIT License. See the LICENSE file for details.
