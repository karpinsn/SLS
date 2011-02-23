# Minimal VR Juggler+OpenSourceGraph Build
# CMake cross-platform build system recipe
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6.3)

# Set package properties
project(Holoviewer)

###
# Configure Dependencies
###
# Locally-developed modules dist'ed with this app - always have this first.
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../CMakeModules")
include(CreateLaunchers)

# VR Juggler
find_package(VRJuggler30 REQUIRED)
include_directories(${VRJUGGLER30_INCLUDE_DIRS})
list(APPEND RUNTIME_LIBRARY_DIRS ${VRJUGGLER30_RUNTIME_LIBRARY_DIRS})
add_definitions(${VRJUGGLER30_DEFINITIONS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VRJUGGLER30_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VRJUGGLER30_CXX_FLAGS}")

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# OpenCV
find_package(OpenCV)
include_directories(${OPENCV_INCLUDE_DIRS})
#include_directories("/usr/include/opencv")

###
# Build the project
###
set(SOURCES
  mainVRJ.cpp
  #mainGLUT.h
  MainWindowController.cpp
  Holodecoder.cpp
  TriMesh.cpp
  ImageIO.cpp
  Camera.cpp
  ShaderFacade.cpp
	TextureFacade.cpp
	FBOFacade.cpp
	OGLStatus.cpp
	Logger.cpp
	Arcball.cpp

	MainWindowController.h
	Holodecoder.h
	TriMesh.h
	ImageIO.h
	Camera.h
	ShaderFacade.h
	TextureFacade.h
	FBOFacade.h
	OGLStatus.h
	Logger.h
	Arcball.h

	glm/glm.hpp
	glm/gtc/matrix_transform.hpp
	glm/gtc/quaternion.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
#	${VRJUGGLER30_LIBRARIES} ${GLEW_LIBRARY}  "cv" "cvaux" "cxcore" "highgui")
	${VRJUGGLER30_LIBRARIES} ${GLEW_LIBRARY} ${OPENCV_LIBRARIES})

create_default_target_launcher(${PROJECT_NAME}
	ARGS standalone.jconf
	FORWARD_ARGS
	RUNTIME_LIBRARY_DIRS
	${RUNTIME_LIBRARY_DIRS}
	ENVIRONMENT
	${VRJUGGLER22_ENVIRONMENT})
