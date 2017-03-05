cmake_minimum_required(VERSION 2.8)

# Project Name
PROJECT(engine)

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

set(TXML_DIR tinyxml2)
set(HEADER_DIR headers)

include_directories (${TXML_DIR} ${HEADER_DIR})

# Include Directories
# In GCC, this will invoke the "-I" command

set( LEARN_CMAKE_SOURCES 
  engine.cpp
  vertex.cpp
)

add_executable(${PROJECT_NAME} ${LEARN_CMAKE_SOURCES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

PROJECT(generator)

set(HEADER_DIR headers)
include_directories (${TXML_DIR} ${HEADER_DIR})

set( LEARN_CMAKE_SOURCES 
  generator.cpp
  shapes.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(${PROJECT_NAME} ${LEARN_CMAKE_SOURCES})


