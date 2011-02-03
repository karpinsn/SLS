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
#include "Texture.h"
#include "OGLStatus.h"

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
                                vrj::GlContextData<GLuint>  vrjVBOId;
                                #define m_vboId             (*vrjVBOId)
			#else
                                GLuint m_vboId;
			#endif
			
		public:
                        VBO(void);
                        ~VBO();
			
			bool init();
			
                        void draw();
			
		private:
                        void _bind(void);
                        void _unbind(void);
			void _initFBO(void);
			void _cacheQuad(void);
		};
	}
}

#endif	// _WRENCH_GL_VBO_H_
