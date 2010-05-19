#ifndef _ShaderFacade_H
#define _ShaderFacade_H

#include <glew.h>

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
	
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;
    bool b_shader_created;

	char* _loadShaderSource(const string &filename);
	bool _validateShader(GLuint shader, const string &filename);
	bool _validateProgram(GLuint program);
};

#endif
