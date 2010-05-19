#	-----------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	-----------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules )

# Set package properties
project(Holoencoder)

###
# CMake Modules Paths

###
# Perform build configuration of dependencies

# Set up QT4
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(DevIL REQUIRED)

include(${QT_USE_FILE})

#	-----------------------------------------------------------------------------
#	List all of the sources to be compiled
#	-----------------------------------------------------------------------------

set(SOURCES
	
	Application.cpp
	MainWindow.cpp
	OpenGLWidget.cpp
	ShaderFacade.cpp
	Holoencoder.cpp
	Camera.cpp
	PointCloudMesh.cpp
	ImageIO.cpp
	
	MainWindow.h
	OpenGLWidget.h
	ShaderFacade.h
	Holoencoder.h
	AOpenGLContext.h
	Camera.h
	Matrix.h
	Vector.h
	PointCloudMesh.h
	ImageIO.h
	
	glm/glm.hpp
	glm/gtc/matrix_transform.hpp
	)
	
set(UIS ui/Holoencoder.ui)	
	
set(MOCS
	MainWindow.h)	

#	-----------------------------------------------------------------------------
#							Qt generation
#	-----------------------------------------------------------------------------

# Let QT pre-process the files and add the generated files to the source list
qt4_wrap_ui(GENERATED_SOURCES ${UIS})
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

# Make sure the compiler can find the pre-processed files from qt4_wrap_ui
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${IL_INCLUDE_DIR})
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}") 

#	-----------------------------------------------------------------------------
#							Copy Shaders to build
#	-----------------------------------------------------------------------------

message(STATUS "Copying shaders to the build directory")

get_target_property(APP_LOCATION ${PROJECT_NAME} LOCATION)
message(${PROJECT_BINARY_DIR})
get_filename_component(APP_PATH ${APP_LOCATION} PATH)
message(${APP_PATH}/../Shaders/)

add_custom_command(TARGET ${PROJECT_NAME}
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND}
				   ARGS -E make_directory ${APP_PATH}/../Shaders/
				   COMMENT "Creating directory: ${APP_PATH}/../Shaders/")

message(STATUS "Looking for shaders in the following directory: " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SHADERS_TO_COPY
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/ "Shaders/*.frag" "Shaders/*.vert")

message(STATUS "Copying the following shaders: " ${SHADERS_TO_COPY})
foreach(FILENAME ${SHADERS_TO_COPY})
   	set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/${FILENAME}")
   	set(DST "${APP_PATH}/../Shaders/${FILENAME}")

	message("SRC " ${SRC})
	message("DST " ${DST})

	# Copy the shader to the directory
   	add_custom_command(TARGET ${PROJECT_NAME}
   					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} 
    				   ARGS -E copy ${SRC} ${DST})
endforeach(FILENAME)

#	-----------------------------------------------------------------------------
#							Build the app
#	-----------------------------------------------------------------------------

# Build and link the app!

# The executable must be a different name than the project for Eclipse
set(EXECUTABLE_NAME ${PROJECT_NAME}Main) 

add_executable(${EXECUTABLE_NAME}
	MACOSX_BUNDLE
	WIN32
	${SOURCES}
	${GENERATED_SOURCES}
	${HEADERS}
	${UIS}
	${MOCS}
	${RESOURCES})

target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${QT_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES})

install (TARGETS ${EXECUTABLE_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )