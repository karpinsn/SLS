#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	---------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

#	-----------------------------------------
# 	Set package properties
#	-----------------------------------------
project(SLS-Desktop)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../../CMakeModules")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

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
add_subdirectory(../../Libraries/wrench/src/wrench ${PROJECT_BINARY_DIR}/Libraries/wrench)
include_directories(../../Libraries/wrench/src)
set(LIBS ${LIBS} wrench)

#	-----------------------------------------
#	Lens
#	-----------------------------------------
add_subdirectory(../../Libraries/lens/src/lens ${PROJECT_BINARY_DIR}/Libraries/lens)
include_directories(../../Libraries/lens/src)
include_directories(${LENS_INCLUDE_DIRS})

set(LIBS ${LIBS} lens "${LENS_LIBRARIES}")

#	-----------------------------------------
#	Antenna
#	-----------------------------------------
add_subdirectory(../../Libraries/antenna/src/antenna ${PROJECT_BINARY_DIR}/Libraries/antenna)
include_directories(../../Libraries/antenna/src)
set(LIBS ${LIBS} antenna)

#	-----------------------------------------
#	Common
#	-----------------------------------------
include_directories(../../Common/src)

#	---------------------------------------------------------------------------------------
#	Generate Documentation
#	---------------------------------------------------------------------------------------
option(SLS_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(SLS_GENERATE_DOC)
	add_subdirectory(../doc ${PROJECT_BINARY_DIR}/doc)
endif(SLS_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(SOURCES
	
	Application.cpp
	MainWindowView.cpp
	MainWindowController.cpp
	OpenGLWidget.cpp
    EncodingOpenGLWidget.cpp
    Holoencoder.cpp
    XYZFileIO.cpp
	XYZMesh.cpp
	MultiWavelengthCapture.cpp
	CaptureController.cpp
    CameraCapture.cpp
    ViewController.cpp
    EncoderController.cpp
    ImageBuffer.cpp
    FrameCapture.cpp
    CameraConnectDialog.cpp
	DepthmapCodecOptionsController.cpp

	MainWindowView.h
	MainWindowController.h
	OpenGLWidget.h
    EncodingOpenGLWidget.h
	Holoencoder.h
	XYZFileIO.h
	XYZMesh.h
	MultiWavelengthCapture.h
	CaptureController.h
    CameraCapture.h
    ViewController.h
    EncoderController.h
    ImageBuffer.h
    FrameCapture.h
    CameraConnectDialog.h
	DepthmapCodecOptionsController.h
	)	

SET(SOURCES_COMMON_HEADERS
	../../Common/src/ImageIO.h
    ../../Common/src/VideoIO.h
	../../Common/src/XYZPoint.h
	../../Common/src/AbstractMesh.h
	../../Common/src/PointCloudMesh.h
	../../Common/src/TriMesh.h
	../../Common/src/AbstractGLContext.h
	../../Common/src/EncodingGLContext.h
    ../../Common/src/Holodecoder.h
	../../Common/src/MeshInterchange.h
)	

SET(SOURCES_COMMON_CODECS_HEADERS
	../../Common/src/Codecs/HolovideoCodec.h
	../../Common/src/Codecs/MultiWavelengthCodec.h
	../../Common/src/Codecs/Codec.h
	../../Common/src/Codecs/DepthCodec.h
)

SET(SOURCES_COMMON_SOURCES
	../../Common/src/ImageIO.cpp
    ../../Common/src/VideoIO.cpp
	../../Common/src/PointCloudMesh.cpp		
	../../Common/src/TriMesh.cpp
    ../../Common/src/Holodecoder.cpp
	../../Common/src/MeshInterchange.cpp
)

SET(SOURCES_COMMON_CODECS_SOURCES
	../../Common/src/Codecs/HolovideoCodec.cpp
	../../Common/src/Codecs/MultiWavelengthCodec.cpp
	../../Common/src/Codecs/DepthCodec.cpp
)
	
# QT Files
set(UIS ui/Holoencoder.ui ui/CameraConnectDialog.ui ui/View.ui ui/Encode.ui ui/Capture.ui ui/DepthmapCodecOptions.ui)
set(MOCS MainWindowView.h MainWindowController.h OpenGLWidget.h EncodingOpenGLWidget.h CaptureController.h ViewController.h EncoderController.h CaptureController.h FrameCapture.h CameraConnectDialog.h DepthmapCodecOptionsController.h)
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
	${SOURCES}
	${SOURCES_COMMON_SOURCES}
	${SOURCES_COMMON_HEADERS}
	${SOURCES_COMMON_CODECS_SOURCES}
	${SOURCES_COMMON_CODECS_HEADERS}
	${GENERATED_SOURCES}
	${HEADERS}
	${UIS}
	${MOCS}
	${RESOURCES}
	${SHADERS_QUALIFIED})

add_dependencies(${PROJECT_NAME} wrench)
add_dependencies(${PROJECT_NAME} lens)

target_link_libraries(${PROJECT_NAME} ${LIBS})

#	---------------------------------------------------------------------------------------
#							Install
#	---------------------------------------------------------------------------------------

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )

#	---------------------------------------------------------------------------------------
#							Copy Shaders to build
#	---------------------------------------------------------------------------------------

add_subdirectory(Shaders)

set(SHADERS_QUALIFIED)
foreach(shader ${SHADER_SOURCES})
	list(APPEND SHADERS_QUALIFIED "Shaders/${shader}")
endforeach()

source_group("Shaders" FILES ${SHADERS_QUALIFIED})

source_group("Common/Source Files" FILES ${SOURCES_COMMON_SOURCES})
source_group("Common/Header Files" FILES ${SOURCES_COMMON_HEADERS})
source_group("Common/Codecs/Source Files" FILES ${SOURCES_COMMON_CODECS_SOURCES})
source_group("Common/Codecs/Header Files" FILES ${SOURCES_COMMON_CODECS_HEADERS})
