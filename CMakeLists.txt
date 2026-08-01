cmake_minimum_required(VERSION 3.31)
project(Snake3D)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

file(COPY assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

add_subdirectory(src)
