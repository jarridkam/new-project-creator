cmake_minimum_required(VERSION 3.20)
project(template_game LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
    GIT_SHALLOW TRUE
)

FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(clay
    URL https://raw.githubusercontent.com/nicbarker/clay/v0.14/clay.h
    DOWNLOAD_NO_EXTRACT TRUE
)

FetchContent_MakeAvailable(raylib json clay)

add_executable(template_game
    src/main.cpp
    src/game.cpp
    src/world/tilemap.cpp
)

target_include_directories(template_game PRIVATE "${clay_SOURCE_DIR}")

target_link_libraries(template_game PRIVATE raylib nlohmann_json::nlohmann_json)
