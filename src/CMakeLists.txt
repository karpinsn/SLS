#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#	---------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(Holoencoder)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/../CMakeModules")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${PROJECT_NAME}")

#	-----------------------------------------
# 	Perform build configuration of dependencies
find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(DevIL REQUIRED)

include(${QT_USE_FILE})

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(SOURCES
	
	Application.cpp
	MainWindow.cpp
	OpenGLWidget.cpp
	ShaderFacade.cpp
	Holoencoder.cpp
	Camera.cpp
	ImageIO.cpp
	XYZFileIO.cpp
	XYZMesh.cpp
	
	MainWindow.h
	OpenGLWidget.h
	ShaderFacade.h
	Holoencoder.h
	Camera.h
	ImageIO.h
	XYZFileIO.h
	XYZMesh.h
	XYZPoint.h
	
	glm/glm.hpp
	glm/gtc/matrix_transform.hpp
	)
	
set(UIS ui/Holoencoder.ui)	
	
set(MOCS MainWindow.h)	

#	---------------------------------------------------------------------------------------
#							Qt generation
#	---------------------------------------------------------------------------------------

# Let QT pre-process the files and add the generated files to the source list
qt4_wrap_ui(GENERATED_SOURCES ${UIS})
qt4_wrap_cpp(GENERATED_SOURCES ${MOCS})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})

# Make sure the compiler can find the pre-processed files from qt4_wrap_ui
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${IL_INCLUDE_DIR})
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

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${QT_LIBRARIES} ${IL_LIBRARIES} ${ILU_LIBRARIES})

#	---------------------------------------------------------------------------------------
#							Copy Shaders to build
#	---------------------------------------------------------------------------------------

message(STATUS "Setting up shader copy commands")

set(SHADER_OUTPUT "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Shaders")
message("Shaders will be copied to: ${SHADER_OUTPUT}")

add_custom_command(TARGET ${PROJECT_NAME}
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND}
				   ARGS -E make_directory ${SHADER_OUTPUT}
				   COMMENT "Creating directory: ${SHADER_OUTPUT}")

message(STATUS "Looking for shaders in the following directory: " ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SHADERS_TO_COPY
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/" 
		"Shaders/*.frag" "Shaders/*.vert")

message(STATUS "Copying the following shaders: " ${SHADERS_TO_COPY})
foreach(FILENAME ${SHADERS_TO_COPY})
   	set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/${FILENAME}")
   	set(DST "${SHADER_OUTPUT}/${FILENAME}")

	message("SRC " ${SRC})
	message("DST " ${DST})

	# Copy the shader to the directory
   	add_custom_command(TARGET ${PROJECT_NAME}
   					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} 
    				   ARGS -E copy "${SRC}" "${DST}"
					   COMMENT "Copying ${SRC} to ${DST}")
endforeach(FILENAME)

#	---------------------------------------------------------------------------------------
#							Install
#	---------------------------------------------------------------------------------------

install (TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/
		BUNDLE DESTINATION ./ )