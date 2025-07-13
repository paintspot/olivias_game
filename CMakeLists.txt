cmake_minimum_required(VERSION 3.10)
project(OliviasGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# This line enables use of SFML_ROOT for find_package
cmake_policy(SET CMP0074 NEW)

# Point to your SFML 2.6.2 installation
set(SFML_DIR "C:/Users/Korvfabriken/Documents/libraries/SFML-2.6.2/lib/cmake/SFML")

# Enable static linking
set(SFML_STATIC_LIBRARIES TRUE)

# Find SFML
find_package(SFML 2.6 COMPONENTS graphics window system main REQUIRED)

link_directories(
    "C:/Users/Korvfabriken/Documents/libraries/SFML-2.6.2/lib"
)

add_executable(main main.cpp)

target_link_libraries(main
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    sfml-main
    freetype
    opengl32
    winmm
    gdi32
)
