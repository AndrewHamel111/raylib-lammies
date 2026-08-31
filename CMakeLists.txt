cmake_minimum_required(VERSION 3.5)
project(raylib-lammies C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

add_compile_definitions(SKIP_MENU)

add_subdirectory(libs/raylib)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(raylib-lammies ${SOURCES})

target_link_libraries(raylib-lammies PRIVATE raylib)

target_include_directories(raylib-lammies PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(raylib-lammies PRIVATE
        -Werror=implicit-function-declaration
        -Werror=builtin-declaration-mismatch
        -Werror=strict-prototypes
        -Wpedantic
)

target_compile_definitions(raylib-lammies PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(raylib-lammies PUBLIC ASSETS_PATH="relative-path-to-assets-in-the-game-package") # Set the asset path macro in release more
