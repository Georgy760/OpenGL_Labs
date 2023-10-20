cmake_minimum_required(VERSION 3.17)
project(lab5)

set(CMAKE_CXX_STANDARD 11)

# Find the required packages
find_package(OpenGL REQUIRED)
find_library(GLUT_LIBRARY NAMES glut PATHS /usr/lib /usr/local/lib)

# Specify the path to the DevIL library
find_package(DevIL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(lab5 main.cpp)

# Link against the required libraries (OpenGL, GLUT, and DevIL)
target_link_libraries(lab5 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} IL)
