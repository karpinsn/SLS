#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#
#	Antenna Library for C++
#	---------------------------------------------------------------------------------------

set(NAME antenna)

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(SOURCES
	
	Transmitter.cpp
	Receiver.cpp

	Transmitter.h
	Receiver.h
)

source_group("antenna" FILES ${SOURCES})

include_directories(..)

#	---------------------------------------------------------------------------------------
#	Build Documentation
#	---------------------------------------------------------------------------------------
option(ANTENNA_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(ANTENNA_GENERATE_DOC)
	add_subdirectory(../../doc ${PROJECT_BINARY_DIR}/doc)
endif(ANTENNA_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

add_library(${NAME} ${SOURCES})

