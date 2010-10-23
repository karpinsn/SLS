/*
 Filename:		ShaderFacade.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _SHADER_FACADE_H_
#define _SHADER_FACADE_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <QtOpenGL/QGLWidget>
#elseif _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <QtOpenGL/QGLWidget>
#else
	#include <GL/glew.h>
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class ShaderFacade 
{
public:
	ShaderFacade();
	ShaderFacade(const char *vsFile, const char *fsFile);
	~ShaderFacade();
	
    bool init(const char *vsFile, const char *fsFile);
    
	void bind();
	void unbind();
	
	unsigned int shaderID();
	
	void uniform(const string name, const int data);
	void uniform(const string name, const float data);
	
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
    //bool b_shader_created;

	char* _loadShaderSource(const string &filename);
	bool _validateShader(GLuint shader, const string &filename);
	bool _validateProgram(GLuint program);
};

#endif	// _SHADER_FACADE_H_
