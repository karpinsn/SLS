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
	
	gl/Arcball.cpp
	gl/Camera.cpp
	gl/FBO.cpp
        #gl/VBO.cpp
	gl/Shader.cpp
	gl/Texture.cpp
	gl/OGLStatus.cpp

	gl/Arcball.h
	gl/Camera.h
	gl/FBO.h
        #gl/VBO.h
	gl/Shader.h
	gl/Texture.h
	gl/OGLStatus.h
)

set(SOURCES
	
	Logger.cpp
	
	Logger.h
)

source_group("wrench" FILES ${SOURCES})
source_group("wrench::gl" FILES ${GL_SOURCES})

include_directories(..)

#	---------------------------------------------------------------------------------------
#	Build the library
#	---------------------------------------------------------------------------------------


add_library(${NAME} ${SOURCES} ${GL_SOURCES})
