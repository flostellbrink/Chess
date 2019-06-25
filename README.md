# Chess [![Build Status](https://travis-ci.com/flostellbrink/Chess.svg?branch=master)](https://travis-ci.com/flostellbrink/Chess)

Project for the Practical Computer Graphics Course at University of Siegen.

Features:
- Runtime generation of geometry from curves.
- Fully functional chess gameplay, including [En Passant](https://en.wikipedia.org/wiki/En_passant) and [Castling](https://en.wikipedia.org/wiki/Castling).
- Ability to undo all moves.
- Dynamic shadows and reflections of the entire environment.
- Overlays with dynamically blurred background.
- Multiple themes and transparency.
- Computer controlled opponent.

## Controls

| Control          | Action                         |
|------------------|--------------------------------|
| Left Click       | Select and move pieces         |
| Drag Right Click | Rotate camera                  |
| Mouse Wheel      | Zoom                           |
| D                | Run demo game                  |
| A                | Toggle AI                      |
| Z                | Undo last move                 |
| T                | Switch theme                   |
| F                | Toggle Fullscreen              |
| N                | New game                       |
| U/J              | Horizontal geometry resolution |
| I/K              | Vertical geometry resolution   |
| Escape/Q         | Quit game                      |

## Installation

60% of the time, it works every time!

### Preparation

1. Setup your C++ compiler of choice.
2. Install [CMake](https://cmake.org/download/)
3. Install [Python](https://www.python.org/downloads/) for Conan (Tested with 3.6)
4. Install [Conan](https://www.conan.io/downloads.html) as admin: `pip install conan`
5. Add Conan remote: `conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan True`

### Dependencies

1. Clone the repository: `git clone --recurse-submodules https://github.com/Owlinated/Chess`
2. Navigate into directory: `cd chess`
3. Get dependencies: `conan install . --build`

### Build

1. Build with conan `conan build .`
