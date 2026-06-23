cmake_minimum_required(VERSION 4.0)
project(stb_image)

add_library(stb_image
    include/stb_image/stb_image.h)
target_sources(stb_image PRIVATE "src/stb_image.cpp")
target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")



