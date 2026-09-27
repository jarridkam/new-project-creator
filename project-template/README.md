# Project template (raylib + nlohmann/json + Clay)

`main.cpp` calls `<ns>::Initialize()`, `Run()`, `Shutdown()` (src/game.h / game.cpp).
`src/world/tilemap.*` loads Tiled JSON maps (.json + .tsj tileset) and draws them; the demo map is in `assets/tilemaps/`.
raylib 6.0, nlohmann/json 3.12.0 and clay 0.14 are downloaded by CMake (`FetchContent`) on the first configure, so the first build needs internet access.

## Replaceable tokens (for the generator)
| Token           | Meaning                        | Files                                   |
|-----------------|--------------------------------|-----------------------------------------|
| `template_game` | project / executable name      | CMakeLists.txt                          |
| `template_game` | primary namespace              | src/main.cpp, src/game.h, src/game.cpp  |
| `TEMPLATE GAME` | window title / on-screen name  | src/game.cpp                            |

## Build
    cmake -B build && cmake --build build && ./build/template_game

Assets are found at `<exe dir>/../assets/`, so build into `build/` directly under the project.
