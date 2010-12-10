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

#ifndef _WRENCH_GL_FBO_H_
#define _WRENCH_GL_FBO_H_

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
		class FBO
		{
		private:
			#ifdef USE_VRJ
				vrj::GlContextData<GLuint> vrjFBOHandle;
				vrj::GlContextData<GLuint> vrjDLHandle;
				#define m_framebuffer   (*vrjFBOHandle)
				#define m_renderingQuad (*vrjDLHandle)
			#else
				GLuint m_framebuffer;
				GLuint m_renderingQuad;
			#endif
			
		public:
			FBO(void);
			~FBO();
			
			bool init();
			
			void bind(void);
			void unbind(void);
			void bindDrawBuffer(GLenum attachmentPoint);
			void setTextureAttachPoint(const Texture &texture, GLenum attachmentPoint);	
			void process(void);
			
		private:
			void _initFBO(void);
			void _cacheQuad(void);
		};
	}
}

#endif	// _WRENCH_GL_FBO_H_