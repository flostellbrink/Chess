# Chess (2015)
Chess project from computer graphics assignment.
Focus of this project is on generation of geometry from curves. All pieces, the clock and the table are generated from curves.
Also notable are reflections of the environment on the chess board and the real time shadows cast by all objects.
Lastly the text drawn over the game will scale according to the games resolution and aspect ratio.

## Build
Use CMake and MinGW.
Tested on Windows 10 with Qt Creator 3.5.1 and Qt 5.4/5.5.

## Controls
| Control          | Action                 | UI Control Alias
|------------------|------------------------|---------------------|
| Left Click       | Select and move pieces |                     |
| Drag Right Click | Move camera            |                     |
| Mouse Wheel      | Zoom                   |                     |
| F                | Toggle Fullscreen      |                     |
| Z                | Undo turn              | Zug zurücknehmen    |
| T                | Switch theme           |                     |
| D                | Run demo game          | Demo starten!       |
| Delete           | Reset game             | Neues Spiel starten |
| Escape/Q         | Quit game              |                     |
|                  | Toggle AI              | Computergegner      |

## UI Controls
First slider controls the generated geometries profile curve resolution. The second slider controls the rotation resolution.
