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

#ifdef USE_VRJ
	#include <vrj/Draw/OGL/GlContextData.h>
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
				vrj::GlContextData<GLuint> vrjShaderID;
				vrj::GlContextData<GLuint> vrjShaderVP;
				vrj::GlContextData<GLuint> vrjShaderFP;
				#define shader_id (*vrjShaderID)
				#define shader_vp (*vrjShaderVP)
				#define shader_fp (*vrjShaderFP)
			#else
				GLuint shader_id;
				GLuint shader_vp;
				GLuint shader_fp;
			#endif
			
		public:
			Shader();
			Shader(const char *vsFile, const char *fsFile);
			~Shader();
			
			bool init(const char *vsFile, const char *fsFile);
			
			void bind();
			void unbind();
			
			unsigned int shaderID();
			
			void uniform(const string name, const int data);
			void uniform(const string name, const float data);
			
		private:
			char* _loadShaderSource(const string &filename);
			bool _validateShader(GLuint shader, const string &filename);
			bool _validateProgram(GLuint program);
		};
	}
}

#endif	// _WRENCH_GL_SHADER_H_
