cmake_minimum_required(VERSION 3.10)
project(OliviasGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

cmake_policy(SET CMP0074 NEW)

set(SFML_DIR "C:/Users/Korvfabriken/Documents/libraries/SFML-2.6.2/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.6 COMPONENTS graphics window system main REQUIRED)

link_directories(
    "C:/Users/Korvfabriken/Documents/libraries/SFML-2.6.2/lib"
)

include_directories(inc)

# Collect all source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    main.cpp
)

# Create the executable
add_executable(OliviasGame ${SOURCES})
set_target_properties(OliviasGame PROPERTIES WIN32_EXECUTABLE TRUE)

# Link SFML libraries
target_link_libraries(OliviasGame
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    sfml-main
    freetype
    opengl32
    winmm
    gdi32
)
