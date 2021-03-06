# Chess [![CI/CD](https://github.com/flostellbrink/Chess/workflows/CI/CD/badge.svg?branch=wasm)](https://github.com/flostellbrink/Chess/actions)

Project for the Practical Computer Graphics Course at University of Siegen.

Features:

-   Runtime generation of geometry from curves.
-   Fully functional chess gameplay, including [En Passant](https://en.wikipedia.org/wiki/En_passant) and [Castling](https://en.wikipedia.org/wiki/Castling).
-   Ability to undo all moves.
-   Dynamic shadows and reflections of the entire environment.
-   Overlays with dynamically blurred background.
-   Multiple themes and transparency.
-   Computer controlled opponent.

## Play Online

[![Preview](images/preview.png)](https://flo.stellbr.ink/portfolio/chess/)

## BrowserStack

Webassembly and WebGL support is somewhat inconsistent across different browsers.

We use BrowserStack to make sure that this game runs on the widest range of devices possible.

[![BrowserStack](images/browserstack-logo-600x315.png)](https://www.browserstack.com/)

## Controls

| Control          | Action                         |
| ---------------- | ------------------------------ |
| Left Click       | Select and move pieces         |
| Drag Right Click | Rotate camera                  |
| Mouse Wheel      | Zoom                           |
| D                | Run demo game                  |
| A                | Toggle AI                      |
| Z                | Undo last move                 |
| T                | Switch theme                   |
| N                | New game                       |
| U/J              | Horizontal geometry resolution |
| I/K              | Vertical geometry resolution   |
| Escape/Q         | Quit game                      |

## Installation

60% of the time, it works every time!

### Preparation

2. Install [CMake](https://cmake.org/download/)
3. Install [Python](https://www.python.org/downloads/) for Conan (Tested with 3.6)
4. Install [Conan](https://www.conan.io/downloads.html) as admin: `pip install conan`
5. Add Conan remote: `conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan True`

### Build

1. Clone the repository: `git clone --recurse-submodules https://github.com/flostellbrink/Chess`
2. Navigate into directory: `cd Chess`
3. Get dependencies: `conan install . --build --profile=emscripten_profile`
4. Build the project: `conan build .`
5. Run dev server: `python3 -m server`
