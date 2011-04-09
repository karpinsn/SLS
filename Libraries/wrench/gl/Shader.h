/*
 Filename:		Shader.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 */

#ifndef _WRENCH_GL_SHADER_H_
#define _WRENCH_GL_SHADER_H_

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

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "OGLStatus.h"

#ifdef USE_VRJ
    #include <vrj/Draw/OpenGL/ContextData.h>
#endif

using namespace std;

namespace wrench
{
	namespace gl 
	{
		class Shader 
		{
		private:
			#ifdef USE_VRJ
                vrj::opengl::ContextData<GLuint> vrjShaderID;
                #define m_shaderID (*vrjShaderID)
			#else
                GLuint m_shaderID;
			#endif
			
		public:
			Shader();
                        Shader(GLenum shaderType, const string &filename);
			~Shader();
			
                        bool init(GLenum shaderType, const string &filename);

                        GLuint shaderID(void);
		private:
			char* _loadShaderSource(const string &filename);
			bool _validateShader(GLuint shader, const string &filename);
		};
	}
}

#endif	// _WRENCH_GL_SHADER_H_
