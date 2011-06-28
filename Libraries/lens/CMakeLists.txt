#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#
#	Lens Library for C++
#	---------------------------------------------------------------------------------------

set(NAME lens) 

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(SOURCES
	Camera.cpp
	CameraObserver.cpp
	OpenCVCamera.cpp
	FileCamera.cpp

	Camera.h
	CameraObserver.h
	OpenCVCamera.h
	FileCamera.h
)

#	-----------------------------------------------
#	Allow the option to compile with JAI Camera
#	-----------------------------------------------
option(LENS_JAI_CAMERA "Build with JAI SDK" OFF)
if(LENS_JAI_CAMERA)
	SET(USE_JAI_CAMERA ON)
	message(STATUS "Building with JAI Camera Functionality")
	LIST(APPEND SOURCES
		JAICamera.cpp
		JAICamera.h
	)
else()
	SET(USE_JAI_CAMERA OFF)
endif()

#	-----------------------------------------------
#	Allow the option to compile with IC Camera
#	-----------------------------------------------
option(LENS_IC_CAMERA "Build with IC_CAMERA" OFF)
if(LENS_IC_CAMERA)
	SET(USE_IC_CAMERA ON)
	message(STATUS "Building with IC Camera Functionality")
	LIST(APPEND SOURCES
		ICCamera.cpp
		ICCamera.h
	)
else()
	SET(USE_IC_CAMERA OFF)
endif()

#	----------------------------------------------
#	Allow the option to compute with Phantom Camera
#	----------------------------------------------
option(LENS_PHANTOM_CAMERA "Build with Phantom Camera" OFF)
if(LENS_PHANTOM_CAMERA)
	SET(USE_PHANTOM_CAMERA ON)
	message(STATUS "Building with Phantom Camera Functionality")
	LIST(APPEND SOURCES
		PhantomCamera.cpp
		PhantomCamera.h
	)
else()
	SET(USE_PHANTOM_CAMERA OFF)
endif()

#	----------------------------------------------
#	Generate the config file for the code
#	----------------------------------------------
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config-lens.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config-lens.h)
LIST(APPEND SOURCES config-lens.h)

#	----------------------------------------------
#	Group the source files
#	----------------------------------------------
source_group("lens" FILES ${SOURCES})
include_directories(..)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

add_library(${NAME} ${SOURCES})

