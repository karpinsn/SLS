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
	ICCamera.cpp	
	OpenCVCamera.cpp
	JAICamera.cpp

	Camera.h
	CameraObserver.h
	ICCamera.h
	OpenCVCamera.h
	JAICamera.h
)

source_group("lens" FILES ${SOURCES})

include_directories(..)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

add_library(${NAME} ${SOURCES})

