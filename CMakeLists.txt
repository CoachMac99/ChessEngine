cmake_minimum_required(VERSION 3.15)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(chess
    src/main.cpp
    src/visualization.cpp
    src/board/bitboard.cpp
    src/moveGeneration/movegen.cpp
)

target_link_libraries(chess PRIVATE
    glad::glad
    glfw
    OpenGL::GL
)