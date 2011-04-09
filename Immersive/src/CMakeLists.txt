#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	---------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

#	-----------------------------------------
# 	Set package properties
#	-----------------------------------------
project(SLS-Immersive)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../../CMakeModules")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

### ---------------------------------------------------------------------------------------
# Configure Dependencies
### ---------------------------------------------------------------------------------------
# Locally-developed modules dist'ed with this app - always have this first.
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../CMakeModules")
include(CreateLaunchers)

#	-----------------------------------------
# 	VR Juggler
#	-----------------------------------------
find_package(VRJuggler30 REQUIRED)
include_directories(${VRJUGGLER30_INCLUDE_DIRS})
list(APPEND RUNTIME_LIBRARY_DIRS ${VRJUGGLER30_RUNTIME_LIBRARY_DIRS})
add_definitions(${VRJUGGLER30_DEFINITIONS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VRJUGGLER30_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VRJUGGLER30_CXX_FLAGS}")
set(LIBS ${LIBS} ${VRJUGGLER30_LIBRARIES})

#	-----------------------------------------
# 	GLEW
#	-----------------------------------------
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
set(LIBS ${LIBS} ${GLEW_LIBRARY})

#	-----------------------------------------
#	OpenCV
#	-----------------------------------------
find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OPENCV_LIBRARIES})

#	----------------------------------------------------------------
#	Libraries
#	----------------------------------------------------------------
include_directories(../../Libraries)
include_directories(../../Libraries/3rdParty)

#	-----------------------------------------
#	GLM
#	-----------------------------------------
add_subdirectory(../../Libraries/3rdParty/glm ${PROJECT_BINARY_DIR}/Libraries/3rdParty/glm)
set(LIBS ${LIBS} glm)

#	-----------------------------------------
#	Wrench
#	-----------------------------------------
add_subdirectory(../../Libraries/wrench ${PROJECT_BINARY_DIR}/Libraries/wrench)
set(LIBS ${LIBS} wrench)

#	-----------------------------------------
#	Common
#	-----------------------------------------
include_directories(../../Common/src)

###
# Build the project
###
set(SOURCES
  mainVRJ.cpp
  MainWindowController.cpp
  ../../Common/src/TriMesh.cpp
  ../../Common/src/ImageIO.cpp
  ../../Common/src/Holodecoder.cpp

  MainWindowController.h
  ../../Common/src/Holodecoder.h
  ../../Common/src/TriMesh.h
  ../../Common/src/ImageIO.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} wrench)
target_link_libraries(${PROJECT_NAME} ${LIBS})

create_default_target_launcher(${PROJECT_NAME}
	ARGS standalone.jconf
	FORWARD_ARGS
	RUNTIME_LIBRARY_DIRS
	${RUNTIME_LIBRARY_DIRS}
	ENVIRONMENT
	${VRJUGGLER30_ENVIRONMENT})

#	---------------------------------------------------------------------------------------
#	Copy Shaders to build
#	---------------------------------------------------------------------------------------

add_subdirectory(Shaders)

set(SHADERS_QUALIFIED)
foreach(shader ${SHADER_SOURCES})
	list(APPEND SHADERS_QUALIFIED "Shaders/${shader}")
endforeach()

source_group("Shaders" FILES ${SHADERS_QUALIFIED})
