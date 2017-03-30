cmake_minimum_required(VERSION 2.8)

# Project Name
PROJECT(cg_phase1)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2)

add_definitions(-std=c++11)

add_executable(engine engine.cpp xmlParser.cpp group.cpp file.cpp tinyxml2/tinyxml2.cpp)
add_executable(generator generator.cpp shapes.cpp)

target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
