#	---------------------------------------------------------------------------------------
# 	2010 Nik Karpinsky
#	LGPL 2010
#
#	Wrench Library for C++
#	---------------------------------------------------------------------------------------

set(NAME wrench)

#	---------------------------------------------------------------------------------------
#	List all of the sources to be compiled
#	---------------------------------------------------------------------------------------

set(GL_SOURCES 
	
	
	gl/Camera.cpp
	gl/FBO.cpp
        gl/ShaderProgram.cpp 
	gl/Shader.cpp
	gl/Texture.cpp
	gl/OGLStatus.cpp
	gl/VBO.cpp
	gl/VAO.cpp
	gl/Converter.cpp

	gl/Camera.h
	gl/FBO.h
        gl/ShaderProgram.h
	gl/Shader.h
	gl/Texture.h
	gl/OGLStatus.h
	gl/VBO.h
	gl/VAO.h
	gl/Converter.h
)

set(GL_UTILS_SOURCES
	gl/utils/GradientBackground.cpp
	gl/utils/Arcball.cpp
	gl/utils/AxisDisplay.cpp

	gl/utils/GradientBackground.h
	gl/utils/Arcball.h
	gl/utils/AxisDisplay.h
)

set(SOURCES
	
	Logger.cpp
	
	Logger.h
)

source_group("wrench" FILES ${SOURCES})
source_group("wrench::gl" FILES ${GL_SOURCES})
source_group("wrench::gl::utils" FILES ${GL_UTILS_SOURCES})

include_directories(..)

#	---------------------------------------------------------------------------------------
#	Build Documentation
#	---------------------------------------------------------------------------------------
option(WRENCH_GENERATE_DOC "Build Doxygen Documentation" OFF)
if(WRENCH_GENERATE_DOC)
	add_subdirectory(../../doc ${PROJECT_BINARY_DIR}/doc)
endif(WRENCH_GENERATE_DOC)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------

add_library(${NAME} ${SOURCES} ${GL_SOURCES} ${GL_UTILS_SOURCES})

