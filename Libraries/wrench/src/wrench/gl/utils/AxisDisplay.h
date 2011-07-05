/**
 *	@filename
 *	@author		Nikolaus Karpinsky
 *
 *	Creates an axis display in the lower left hand corner of the screen, which shows
 *	what the model view axis looks like in the current display. Useful for showing what
 *	what the model view matrix looks like at a point in the scene. To use it, 
 *	simply create an instance, call AxisDislay::init() during initalization and 
 *	then call AxisDisplay::draw(glm::mat4 modelViewMatrix) inside the OpenGL draw thread.
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

#include "../../Logger.h"
#include "../OGLStatus.h"
#include "../VAO.h"
#include "../VBO.h"
#include "../ShaderProgram.h"

#ifdef USE_VRJ
	#include <vrj/Draw/OGL/GlContextData.h>
#endif

namespace wrench 
{
	namespace gl
	{
		namespace utils
		{
                class AxisDisplay
                {
                private:
                    VAO m_axis;		//	Vertex array object which holds the axis lines
                    VBO m_vertex;	//	Vertex buffer object which holds the axis line coordinates
                    VBO m_color;	//	Vertex buffer object which holds the color of the axis lines

                    ShaderProgram m_axisShader;	//	Shader program used to correctly transform the axis

				public:
                    AxisDisplay(void);
                    ~AxisDisplay();
			
					/**
					*	Initializes the shaders used by AxisDisplay along with the
					*	vertex buffer object and vertex array object used to draw the
					*	axis.
					*/
                    bool init();
			
					/**
					*	Draws the model view matrix passed in as an axis in the lower left hand corner.
					*
					*	@param modelViewMatrix	Model view matrix to draw.
					*/
                    void draw(glm::mat4 modelViewMatrix);
				};
		}
	}
}

#endif	// _WRENCH_GL_AXIS_DISPLAY_H_