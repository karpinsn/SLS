/*
 Filename:		FBO.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	10/27/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 12/09/10 - Nik Karpinsky - Moved file into the Wrench library
 */

#ifndef _WRENCH_GL_AXIS_DISPLAY_H_
#define _WRENCH_GL_AXIS_DISPLAY_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <OpenGL/gl.h>
#elif _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <glm/glm.hpp>

#include "../Logger.h"
#include "OGLStatus.h"
#include "VAO.h"
#include "VBO.h"
#include "ShaderProgram.h"

#ifdef USE_VRJ
	#include <vrj/Draw/OGL/GlContextData.h>
#endif

namespace wrench 
{
	namespace gl
	{
                class AxisDisplay
                {
                private:
                    VAO m_axis;
                    VBO m_vertex;
                    VBO m_color;

                    ShaderProgram m_axisShader;

		public:
                    AxisDisplay(void);
                    ~AxisDisplay();
			
                    bool init();
			
                    void draw(glm::mat4 modelViewMatrix);
		};
	}
}

#endif	// _WRENCH_GL_AXIS_DISPLAY_H_
