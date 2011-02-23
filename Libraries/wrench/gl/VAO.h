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

#ifndef _WRENCH_GL_VBA_H_
#define _WRENCH_GL_VBA_H_

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

#include "map"

#include "../Logger.h"
#include "OGLStatus.h"
#include "VBO.h"

#ifdef USE_VRJ
	#include <vrj/Draw/OGL/GlContextData.h>
#endif

namespace wrench 
{
	namespace gl
	{
                class VAO
		{
		private:
			#ifdef USE_VRJ
                                vrj::GlContextData<GLuint>  vrjVAOID;
                                #define m_vaoID             (*vrjVAOID)
			#else
                                GLuint m_vaoID;
			#endif
			
                        GLenum  m_mode;     // Defined by glDrawArrays: GL_TRIANGLE_STRIP, GL_QUAD, etc...
                        GLsizei m_count;    // Defined by glDrawArrays: Number of things to draw

                        std::map<std::string, int> m_vertexAttributes;
		public:
                        VAO(void);
                        ~VAO();
			
                        bool init(GLenum mode, GLsizei count);
                        void addVBO(VBO& vbo, std::string attributeName);
                        GLuint getVertexAttributeAddress(std::string attributeName);

                        void draw();
			
		private:
                        void _bind(void);
                        void _unbind(void);
		};
	}
}

#endif	// _WRENCH_GL_VAO_H_
