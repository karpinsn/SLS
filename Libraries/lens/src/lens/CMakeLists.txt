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
	
	find_package(JAI REQUIRED)

	if(JAI_FOUND)	
		include_directories(${JAI_INCLUDE_DIR})
		LIST(APPEND LIBS ${JAI_LIBRARIES})

		# Add the include dirs and libraries to the Lens files
		SET(LENS_INCLUDE_DIRS ${LENS_INCLUDE_DIRS} ${JAI_INCLUDE_DIR})
		SET(LENS_LIBRARIES ${LENS_LIBRARIES} ${JAI_LIBRARIES})

		LIST(APPEND SOURCES
			JAICamera.cpp
			JAICamera.h
		)
	endif(JAI_FOUND)
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
	
	find_package(Phantom REQUIRED)

	if(PHANTOM_FOUND)
		include_directories(${PHANTOM_INCLUDE_DIR})
		LIST(APPEND LIBS ${PHANTOM_LIBRARIES})

		SET(LENS_INCLUDE_DIRS ${LENS_INCLUDE_DIRS} ${PHANTOM_INCLUDE_DIR})
		SET(LENS_LIBRARIES ${LENS_LIBRARIES} ${PHANTOM_LIBRARIES})

		LIST(APPEND SOURCES
			PhantomCamera.cpp
			PhantomCamera.h
		)
	endif(PHANTOM_FOUND)
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
#	Generate Documentation
#	---------------------------------------------------------------------------------------
option(LENS_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(LENS_GENERATE_DOC)
	add_subdirectory(../../doc ${PROJECT_BINARY_DIR}/doc)
endif(LENS_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

set(LENS_INCLUDE_DIRS ${LENS_INCLUDE_DIRS} PARENT_SCOPE)
set(LENS_LIBRARIES ${LENS_LIBRARIES} PARENT_SCOPE)

add_library(${NAME} ${SOURCES})

