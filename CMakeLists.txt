cmake_minimum_required(VERSION 3.17)
project(labs)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

# Specify the path to the GLUT library
find_library(GLUT_LIBRARY NAMES glut PATHS /usr/lib /usr/local/lib)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(labs lab1.cpp
        lab2.cpp)

target_link_libraries(labs ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
