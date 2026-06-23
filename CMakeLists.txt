cmake_minimum_required(VERSION 3.16)

project(Terraria)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(Terraria ${SRC_FILES})

target_link_libraries(Terraria PRIVATE
    opengl32
    gdi32
    winmm
)