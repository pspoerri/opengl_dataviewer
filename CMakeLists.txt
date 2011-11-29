# Note set CUDA_BUILD_EMULATION to OFF to enable Device mode compilation.

cmake_minimum_required(VERSION 2.6.2)

project(OpenGL_Dataviewer)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
#set(CMAKE_VERBOSE_MAKEFILE YES)

#### QT
find_package(Qt4 REQUIRED)

#### Glut and OpenGL
find_package(GLUT)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

#### Only require GLUT but not necessarily Xmu
if(NOT GLUT_FOUND)
message(FATAL_ERROR "Could not find GLEW.")
endif(NOT GLUT_FOUND)



include(${QT_USE_FILE})
include_directories(${QT_INCLUDES} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src)

set(MAIN_SRCS
  src/main.c
)

add_executable(Dataviewer ${MAIN_SRCS})
set_source_files_properties(${MAIN_SRCS} PROPERTIES LANGUAGE CXX)
target_link_libraries(Dataviewer ${QT_LIBRARIES} ${GLUT_glut_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})

