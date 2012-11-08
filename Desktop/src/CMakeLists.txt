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
add_subdirectory(../../Libraries/3rdParty/glm/glm ${PROJECT_BINARY_DIR}/Libraries/3rdParty/glm/glm)
include_directories(../../Libraries/3rdParty/glm)
set(LIBS ${LIBS} glm)

#	-----------------------------------------
#	Reactor
#	-----------------------------------------
add_subdirectory(../../Libraries/Reactor/src/reactor ${PROJECT_BINARY_DIR}/Libraries/Reactor)
include_directories(../../Libraries/Reactor/src)
include_directories(${REACTOR_INCLUDE_DIRS})
set(LIBS ${LIBS} reactor "${REACTOR_LIBRARIES}")

#	-----------------------------------------
#	Wrench
#	-----------------------------------------
add_subdirectory(../../Libraries/Wrench/src/wrench ${PROJECT_BINARY_DIR}/Libraries/Wrench)
include_directories(../../Libraries/Wrench/src)
set(LIBS ${LIBS} wrench)

#	-----------------------------------------
#	Lens
#	-----------------------------------------
add_subdirectory(../../Libraries/Lens/src/lens ${PROJECT_BINARY_DIR}/Libraries/Lens)
include_directories(../../Libraries/Lens/src)
include_directories(${LENS_INCLUDE_DIRS})
set(LIBS ${LIBS} lens "${LENS_LIBRARIES}")

#	-----------------------------------------
#	Antenna
#	-----------------------------------------
add_subdirectory(../../Libraries/Antenna/src/antenna ${PROJECT_BINARY_DIR}/Libraries/Antenna)
include_directories(../../Libraries/Antenna/src)
include_directories(${ANTENNA_INCLUDE_DIRS})
set(LIBS ${LIBS} antenna "${ANTENNA_LIBRARIES}")

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
    ViewController.cpp
    EncoderController.cpp
    ImageBuffer.cpp
    
	Application.h
	MainWindowView.h
	MainWindowController.h
	OpenGLWidget.h
    EncodingOpenGLWidget.h
	Holoencoder.h
	XYZFileIO.h
	XYZMesh.h
    ViewController.h
    EncoderController.h
    ImageBuffer.h
	)	

SET(SOURCES_ENCODER
    Encoder/DepthmapCodecOptionsController.cpp
	Encoder/SixFringeCodecOptionsController.cpp
    Encoder/NineFringeCodecOptionsController.cpp
    Encoder/HolovideoCodecOptionsController.cpp
	Encoder/HoloimageEncoderOptionsController.cpp
    Encoder/XYZMCodecOptionsController.cpp
    Encoder/HolovideoEncoderOptionsController.cpp

    Encoder/DepthmapCodecOptionsController.h
	Encoder/SixFringeCodecOptionsController.h
    Encoder/NineFringeCodecOptionsController.h
    Encoder/HolovideoCodecOptionsController.h
	Encoder/HoloimageEncoderOptionsController.h
    Encoder/XYZMCodecOptionsController.h
    Encoder/HolovideoEncoderOptionsController.h
)

SET(SOURCES_CAPTURE
	Capture/SaveStream.cpp
	Capture/CameraCapture.cpp
	Capture/CaptureController.cpp
	Capture/CameraConnectDialog.cpp
	Capture/CaptureTypeSelectDialog.cpp
	Capture/FrameCapture.cpp
	Capture/FileOutputStream.cpp
    Capture/WebsocketOutputStream.cpp
	Capture/CaptureGLWidget.cpp

    Capture/WebsocketOutputStream.h
	Capture/FileOutputStream.h
	Capture/IOutputStream.h
	Capture/SaveStream.h
	Capture/ICapture.h
	Capture/CameraCapture.h
	Capture/CaptureController.h
	Capture/CameraConnectDialog.h
	Capture/CaptureTypeSelectDialog.h
	Capture/FrameCapture.h
	Capture/CaptureGLWidget.h
)

SET(SOURCES_THREEFRINGE
	Capture/ThreeFringe/ThreeFringeCapture.cpp
	Capture/ThreeFringe/ComputePhase.cpp
	Capture/ThreeFringe/FastConnectComponent.cpp
	Capture/ThreeFringe/ThresholdedPhaseUnwrap.cpp
	Capture/ThreeFringe/TrapezoidalLUTPhaseWrap.cpp
	
	Capture/ThreeFringe/ThreeFringeCapture.h
	Capture/ThreeFringe/ComputePhase.h
	Capture/ThreeFringe/FastConnectComponent.h
	Capture/ThreeFringe/ThresholdedPhaseUnwrap.h
	Capture/ThreeFringe/TrapezoidalLUTPhaseWrap.h
)

SET(SOURCES_SIXFRINGE
	Capture/SixFringe/SixFringeCapture.cpp
	Capture/SixFringe/SixFringeCapture.h
)

SET(SOURCES_NINEFRINGE
	Capture/NineFringe/NineFringeCapture.cpp
	Capture/NineFringe/NineFringeCapture.h
)

SET(SOURCES_COMMON_HEADERS
	../../Common/src/ImageIO.h
    ../../Common/src/VideoIO.h
	../../Common/src/XYZPoint.h
	../../Common/src/AbstractMesh.h
	../../Common/src/PointCloudMesh.h
	../../Common/src/TriMesh.h
	../../Common/src/IGLContext.h
	../../Common/src/IEncoder.h
	../../Common/src/IDecoder.h
	../../Common/src/Holodecoder.h
	../../Common/src/MeshInterchange.h
)	

SET(SOURCES_COMMON_CODECS_HEADERS
	../../Common/src/Codecs/Codec.h
    ../../Common/src/Codecs/HolovideoEncoder.h
    ../../Common/src/Codecs/HoloimageEncoder.h
	../../Common/src/Codecs/HolovideoDecoder.h
	../../Common/src/Codecs/SixFringeDecoder.h
	../../Common/src/Codecs/NineFringeDecoder.h
	../../Common/src/Codecs/DepthEncoder.h
    ../../Common/src/Codecs/XYZMDecoder.h
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
	../../Common/src/Codecs/HolovideoEncoder.cpp
	../../Common/src/Codecs/HoloimageEncoder.cpp
    ../../Common/src/Codecs/HolovideoDecoder.cpp
	../../Common/src/Codecs/SixFringeDecoder.cpp
	../../Common/src/Codecs/NineFringeDecoder.cpp
	../../Common/src/Codecs/DepthEncoder.cpp
    ../../Common/src/Codecs/XYZMDecoder.cpp
)
	
# QT Files
set(UIS 
	ui/MainWindow.ui 
	ui/CameraConnectDialog.ui 
	ui/CaptureTypeSelectDialog.ui 
	ui/View.ui 
	ui/Encode.ui 
	ui/Capture.ui 
	ui/DepthmapCodecOptions.ui 
	ui/SixFringeCodecOptions.ui
	ui/NineFringeCodecOptions.ui
	ui/HolovideoCodecOptions.ui 
	ui/HoloimageEncoderOptions.ui 
	ui/XYZMCodecOptions.ui 
	ui/HolovideoEncoderOptions.ui)

set(MOCS 
	MainWindowView.h 
	MainWindowController.h 
	OpenGLWidget.h 
	Capture/CaptureGLWidget.h
	EncodingOpenGLWidget.h 
	Capture/CaptureController.h 
	ViewController.h 
	EncoderController.h 
	Capture/FrameCapture.h 
	Capture/CameraConnectDialog.h 
	Capture/CaptureTypeSelectDialog.h 
	Capture/WebsocketOutputStream.h
	Encoder/DepthmapCodecOptionsController.h 
	Encoder/SixFringeCodecOptionsController.h
	Encoder/NineFringeCodecOptionsController.h 
	Encoder/HolovideoCodecOptionsController.h 
	Encoder/HoloimageEncoderOptionsController.h 
	Encoder/XYZMCodecOptionsController.h 
	Encoder/HolovideoEncoderOptionsController.h)
set(RESOURCES ui/MainWindow.qrc)

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
	${SOURCES_ENCODER}
	${SOURCES_CAPTURE}
	${SOURCES_THREEFRINGE}
	${SOURCES_SIXFRINGE}
	${SOURCES_NINEFRINGE}
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

add_dependencies(${PROJECT_NAME} reactor)
add_dependencies(${PROJECT_NAME} wrench)
add_dependencies(${PROJECT_NAME} lens)
add_dependencies(${PROJECT_NAME} antenna)

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

source_group("UI" FILES ${UISUIS})

source_group("Capture" FILES ${SOURCES_CAPTURE})
source_group("Capture/ThreeFringe" FILES ${SOURCES_THREEFRINGE})
source_group("Capture/SixFringe" FILES ${SOURCES_SIXFRINGE})
source_group("Capture/NineFringe" FILES ${SOURCES_NINEFRINGE})

source_group("Common/Source Files" FILES ${SOURCES_COMMON_SOURCES})
source_group("Common/Header Files" FILES ${SOURCES_COMMON_HEADERS})
source_group("Common/Codecs/Source Files" FILES ${SOURCES_COMMON_CODECS_SOURCES})
source_group("Common/Codecs/Header Files" FILES ${SOURCES_COMMON_CODECS_HEADERS})
