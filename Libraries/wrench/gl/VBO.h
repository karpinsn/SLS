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

#ifndef _WRENCH_GL_VBO_H_
#define _WRENCH_GL_VBO_H_

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

#include "../Logger.h"
#include "OGLStatus.h"
#include "Converter.h"

#ifdef USE_VRJ
	#include <vrj/Draw/OGL/GlContextData.h>
#endif

namespace wrench 
{
	namespace gl
	{
                class VBO
		{
		private:
			#ifdef USE_VRJ
                                vrj::GlContextData<GLuint>  vrjVBOID;
                                #define m_vboID             (*vrjVBOID)
			#else
                                GLuint m_vboID;
			#endif
			
                        GLint   m_componentSize;    // Defined in glVertexAttribPointer as size
                        GLenum  m_componentType;    // Defined in glVertexAttribPointer as type
                        GLenum  m_target;           // Defined in glBufferData as target
		public:
                        VBO(void);
                        ~VBO();
			
                        bool init(GLint compSize, GLenum type, GLenum target);  // Here compSize is the number of components per vertex
                        void bufferData(GLsizei size, const GLvoid* data, GLenum usage);

                        GLint getComponentSize(void);
                        GLenum getComponentType(void);
                        GLenum getTarget(void);
                        void bind(void);
                        void unbind(void);
		};
	}
}

#endif	// _WRENCH_GL_VBO_H_
