#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	---------------------------------------------------------------------------------------

set(SOURCES
	MainWindowView.cpp
	MainWindowController.cpp
	OpenGLWidget.cpp
    XYZFileIO.cpp
	XYZMesh.cpp
    ViewController.cpp
    ImageBuffer.cpp

    ImageIO.cpp
    VideoIO.cpp
	PointCloudMesh.cpp		
	TriMesh.cpp
    Holodecoder.cpp
	MeshInterchange.cpp
	)	

file(GLOB
	HEADERS
	../include/*.h)

SET(SOURCES_ENCODER
    Encode/DepthHoloencoder.cpp
    Encode/SpatialHoloencoder.cpp
    Encode/EncoderController.cpp
    Encode/EncodingOpenGLWidget.cpp
    Encode/DepthmapCodecOptionsController.cpp
	Encode/SixFringeCodecOptionsController.cpp
    Encode/NineFringeCodecOptionsController.cpp
    Encode/HolovideoCodecOptionsController.cpp
	Encode/HoloimageEncoderOptionsController.cpp
    Encode/XYZMCodecOptionsController.cpp
    Encode/HolovideoEncoderOptionsController.cpp
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

	Capture/ThreeFringeCapture.cpp
	Capture/ComputePhase.cpp
	Capture/FastConnectComponent.cpp
	Capture/ThresholdedPhaseUnwrap.cpp
	Capture/TrapezoidalLUTPhaseWrap.cpp

	Capture/SixFringeCapture.cpp
	Capture/NineFringeCapture.cpp
)

SET(SOURCES_CODECS
	Codecs/HolovideoEncoder.cpp
	Codecs/HoloimageEncoder.cpp
    Codecs/HolovideoDecoder.cpp
	Codecs/SixFringeDecoder.cpp
	Codecs/NineFringeDecoder.cpp
	Codecs/DepthEncoder.cpp
    Codecs/XYZMDecoder.cpp
)
	
# QT Files
set(UIS 
	../ui/MainWindow.ui 
	../ui/CameraConnectDialog.ui 
	../ui/CaptureTypeSelectDialog.ui 
	../ui/View.ui 
	../ui/Encode.ui 
	../ui/Capture.ui 
	../ui/DepthmapCodecOptions.ui 
	../ui/SixFringeCodecOptions.ui
	../ui/NineFringeCodecOptions.ui
	../ui/HolovideoCodecOptions.ui 
	../ui/HoloimageEncoderOptions.ui 
	../ui/XYZMCodecOptions.ui 
	../ui/HolovideoEncoderOptions.ui)

set(MOCS 
	../include/MainWindowView.h 
	../include/MainWindowController.h 
	../include/OpenGLWidget.h 
	../include/ViewController.h 
	../include/Capture/CaptureGLWidget.h
	../include/Capture/CaptureController.h 
	../include/Capture/FrameCapture.h 
	../include/Capture/CameraConnectDialog.h 
	../include/Capture/CaptureTypeSelectDialog.h 
	../include/Capture/WebsocketOutputStream.h
	../include/Encode/DepthmapCodecOptionsController.h 
    ../include/Encode/EncoderController.h 
    ../include/Encode/EncodingOpenGLWidget.h 
	../include/Encode/SixFringeCodecOptionsController.h
	../include/Encode/NineFringeCodecOptionsController.h 
	../include/Encode/HolovideoCodecOptionsController.h 
	../include/Encode/HoloimageEncoderOptionsController.h 
	../include/Encode/XYZMCodecOptionsController.h 
	../include/Encode/HolovideoEncoderOptionsController.h)

set(RESOURCES ../ui/MainWindow.qrc)

#	---------------------------------------------------------------------------------------
#							Qt generation
#	---------------------------------------------------------------------------------------

# Let QT pre-process the files and add the generated files to the source list
qt4_wrap_ui(GENERATED_SOURCES ${UIS})
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})
qt4_add_resources(GENERATED_SOURCES ${RESOURCES})
message(STATUS ${RESOURCES})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

# Make sure the compiler can find the pre-processed files from qt4_wrap_ui
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}") 

#	---------------------------------------------------------------------------------------
#							Add the library
#	---------------------------------------------------------------------------------------

add_library("${PROJECT_NAME}"
	${SOURCES}
	${HEADERS}

	${SOURCES_ENCODER}
	${SOURCES_CAPTURE}
	${SOURCES_CODECS}
	${GENERATED_SOURCES}
	${UIS}
	${MOCS}
	${RESOURCES}
	${SHADERS_QUALIFIED}
)

target_link_libraries(${PROJECT_NAME} ${LIBS})

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

source_group("Encode" FILES ${SOURCES_ENCODER})

source_group("Common/Source Files" FILES ${SOURCES_COMMON_SOURCES})
source_group("Common/Header Files" FILES ${SOURCES_COMMON_HEADERS})
source_group("Common/Codecs/Source Files" FILES ${SOURCES_COMMON_CODECS_SOURCES})
source_group("Common/Codecs/Header Files" FILES ${SOURCES_COMMON_CODECS_HEADERS})

#add_dependencies(${PROJECT_NAME} reactor)
#add_dependencies(${PROJECT_NAME} wrench)
#add_dependencies(${PROJECT_NAME} lens)
#add_dependencies(${PROJECT_NAME} antenna)
