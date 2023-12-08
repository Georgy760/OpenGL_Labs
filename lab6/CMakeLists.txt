cmake_minimum_required(VERSION 3.17)
project(lab6)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

# Specify the path to the GLUT library
find_library(GLUT_LIBRARY NAMES glut PATHS /usr/lib /usr/local/lib)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(lab6 main.cpp
)

target_link_libraries(lab6 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})