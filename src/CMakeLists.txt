cmake_minimum_required(VERSION 2.8)

# Project Name
PROJECT(cg_phase1)

if (NOT APPLE)
	if (WIN32)
		set(GLEW_ROOT_FOLDER "" CACHE PATH "Path to GLEW folder")
		include_directories(${GLEW_ROOT_FOLDER})
		link_directories(${GLEW_ROOT_FOLDER})
	else(WIN32)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif(WIN32)
endif(NOT APPLE)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2)

add_definitions(-std=c++11)

add_executable(engine engine.cpp xmlParser.cpp group.cpp file.cpp transforms.cpp tinyxml2/tinyxml2.cpp)
add_executable(generator generator.cpp shapes.cpp)

if (WIN32)
	target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glew32)
else(WIN32)
	target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif (WIN32)

