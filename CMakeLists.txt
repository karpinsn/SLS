#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	---------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/CMakeModules")

#	-----------------------------------------
# 	Set package properties
#	-----------------------------------------
project(SLS)
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")
set(LENS_FILE_CAMERA ON BOOL) # This is set as SLS requires the file camera functionality

### ---------------------------------------------------------------------------------------
# Configure Dependencies
### ---------------------------------------------------------------------------------------

#	-----------------------------------------
#	QT
#	-----------------------------------------
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
include(${QT_USE_FILE})
set (LIBS ${LIBS} ${QT_LIBRARIES})

#	-----------------------------------------
#	OpenGL
#	-----------------------------------------
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

#	-----------------------------------------
#	OpenCV
#	-----------------------------------------
find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenCV_LIBS})

#	-----------------------------------------
#	GLEW
#	-----------------------------------------
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
set(LIBS ${LIBS} ${GLEW_LIBRARY})

#	----------------------------------------------------------------
#	Libraries
#	----------------------------------------------------------------
include_directories(Libraries)
include_directories(Libraries/3rdParty)

#	-----------------------------------------
#	GLM
#	-----------------------------------------
add_subdirectory(Libraries/3rdParty/glm/glm ${PROJECT_BINARY_DIR}/Libraries/3rdParty/glm/glm)
include_directories(Libraries/3rdParty/glm)
set(LIBS ${LIBS} glm)

#	-----------------------------------------
#	Reactor
#	-----------------------------------------
add_subdirectory(Libraries/Reactor/src/reactor ${PROJECT_BINARY_DIR}/Libraries/Reactor)
include_directories(Libraries/Reactor/src)
include_directories(${REACTOR_INCLUDE_DIRS})
set(LIBS ${LIBS} reactor "${REACTOR_LIBRARIES}")

#	-----------------------------------------
#	Wrench
#	-----------------------------------------
add_subdirectory(Libraries/Wrench/src/wrench ${PROJECT_BINARY_DIR}/Libraries/Wrench)
include_directories(Libraries/Wrench/src)
set(LIBS ${LIBS} wrench)

#	-----------------------------------------
#	Lens
#	-----------------------------------------
add_subdirectory(Libraries/Lens/src/lens ${PROJECT_BINARY_DIR}/Libraries/Lens)
include_directories(Libraries/Lens/src)
include_directories(${LENS_INCLUDE_DIRS})
set(LIBS ${LIBS} lens "${LENS_LIBRARIES}")

#	-----------------------------------------
#	Antenna
#	-----------------------------------------
add_subdirectory(Libraries/Antenna/src/antenna ${PROJECT_BINARY_DIR}/Libraries/Antenna)
include_directories(Libraries/Antenna/src)
include_directories(${ANTENNA_INCLUDE_DIRS})
set(LIBS ${LIBS} antenna "${ANTENNA_LIBRARIES}")

#	---------------------------------------------------------------------------------------
#	Generate Documentation
#	---------------------------------------------------------------------------------------
option(SLS_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(SLS_GENERATE_DOC)
	add_subdirectory(doc ${PROJECT_BINARY_DIR}/doc)
endif(SLS_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	Add the sources
#	---------------------------------------------------------------------------------------
include_directories("include")
add_subdirectory(src)
set(LIBS ${LIBS} "${PROJECT_NAME}")


# Make sure the compiler can find the pre-processed files from qt4_wrap_ui
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}") 

#	---------------------------------------------------------------------------------------
#							Build the app
#	---------------------------------------------------------------------------------------

# Build and link the app!
add_executable("${PROJECT_NAME}Launcher"
	MACOSX_BUNDLE
	src/Application.cpp)

add_dependencies("${PROJECT_NAME}Launcher" ${PROJECT_NAME})
target_link_libraries("${PROJECT_NAME}Launcher" ${LIBS})

#	---------------------------------------------------------------------------------------
#							Install
#	---------------------------------------------------------------------------------------

install (TARGETS "${PROJECT_NAME}Launcher"
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )
