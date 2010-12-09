#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	---------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(Holoencoder)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../CMakeModules")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

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
set(LIBS ${LIBS} ${OPENCV_LIBRARIES})

#	-----------------------------------------
#	GLEW
#	-----------------------------------------
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})
set(LIBS ${LIBS} ${GLEW_LIBRARY})

#	-----------------------------------------
#	GLM
#	-----------------------------------------
add_subdirectory(glm)

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(SOURCES
	
	Application.cpp
	MainWindowView.cpp
	MainWindowController.cpp
	OpenGLWidget.cpp
	Holoencoder.cpp
	Camera.cpp
	ImageIO.cpp
	XYZFileIO.cpp
	XYZMesh.cpp
	Holodecoder.cpp
	PointCloudMesh.cpp
	TriMesh.cpp
	Arcball.cpp
	OGLStatus.cpp
	Logger.cpp	
	ShaderFacade.cpp
	FBOFacade.cpp
	TextureFacade.cpp

	MainWindowView.h
	MainWindowController.h
	OpenGLWidget.h
	Holoencoder.h
	Camera.h
	ImageIO.h
	XYZFileIO.h
	XYZMesh.h
	XYZPoint.h
	AbstractMesh.h
	Holodecoder.h
	PointCloudMesh.h
	TriMesh.h
	AbstractGLContext.h	
	Arcball.h
	OGLStatus.h
	Logger.h
	ShaderFacade.h
	FBOFacade.h
	TextureFacade.h
	)
	
#	glm/glm.hpp
#	glm/gtc/matrix_transform.hpp	
	
# QT Files
set(UIS ui/Holoencoder.ui)	
set(MOCS MainWindowView.h MainWindowController.h)	
set(RESOURCES ui/Holoencoder.qrc)

#	---------------------------------------------------------------------------------------
#							Qt generation
#	---------------------------------------------------------------------------------------

# Let QT pre-process the files and add the generated files to the source list
qt4_wrap_ui(GENERATED_SOURCES ${UIS})
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})
qt4_add_resources(GENERATED_SOURCES ${RESOURCES})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

# Make sure the compiler can find the pre-processed files from qt4_wrap_ui
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}") 

#	---------------------------------------------------------------------------------------
#							Build the app
#	---------------------------------------------------------------------------------------

# Build and link the app!

add_executable(${PROJECT_NAME}
	MACOSX_BUNDLE
	WIN32
	${SOURCES}
	${GENERATED_SOURCES}
	${HEADERS}
	${UIS}
	${MOCS}
	${RESOURCES})

message(STATUS "Libraries to link:" ${LIBS} " Done")

target_link_libraries(${PROJECT_NAME} ${LIBS})

#	---------------------------------------------------------------------------------------
#							Copy Shaders to build
#	---------------------------------------------------------------------------------------

add_subdirectory(Shaders)

#	---------------------------------------------------------------------------------------
#							Install
#	---------------------------------------------------------------------------------------

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )