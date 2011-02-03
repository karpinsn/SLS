#include "Shader.h"

wrench::gl::Shader::Shader()
{
    shader_id = 0;
	shader_vp = 0;
	shader_fp = 0;
}

wrench::gl::Shader::Shader(const char *vsFile, const char *fsFile)
{
    shader_id = 0;
	shader_vp = 0;
	shader_fp = 0;
	
    init(vsFile, fsFile);
}

wrench::gl::Shader::~Shader()
{
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);
    
	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

bool wrench::gl::Shader::init(const char *vsFile, const char *fsFile)
{
	bool shaderCreated = false;
	
    if (GLEW_VERSION_2_0)
    {
	    shader_vp = glCreateShader(GL_VERTEX_SHADER);
	    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
        
		const GLchar* vsText = _loadShaderSource(vsFile);
		const GLchar* fsText = _loadShaderSource(fsFile);	
        
        if (vsText == NULL || fsText == NULL) 
		{
            cerr << "Either vertex shader or fragment shader file not found." << endl;
        }
		else
		{
			bool shaderCompileFlag = false;
			
			glShaderSource(shader_vp, 1, &vsText, 0);
			glShaderSource(shader_fp, 1, &fsText, 0);
			
			glCompileShader(shader_vp);
			shaderCompileFlag = _validateShader(shader_vp, vsFile);
			if (!shaderCompileFlag) return false;
			
			glCompileShader(shader_fp);
			shaderCompileFlag = _validateShader(shader_fp, fsFile);
			if (!shaderCompileFlag) return false;
			
			shader_id = glCreateProgram();
			glAttachShader(shader_id, shader_fp);
			glAttachShader(shader_id, shader_vp);
			glLinkProgram(shader_id);
			
			shaderCreated = _validateProgram(shader_id);
		}
		
		if(NULL != vsText)
		{
			delete[] vsText;
		}
		if(NULL != fsText)
		{
			delete[] fsText;
		}
    }
	
	return shaderCreated;
}

unsigned int wrench::gl::Shader::shaderID() 
{
	return shader_id;
}

void wrench::gl::Shader::bind() {
	glUseProgram(shader_id);
}

void wrench::gl::Shader::unbind() 
{
	glUseProgram(0);
}

void wrench::gl::Shader::uniform(const string name, const int data)
{
	bind();
	GLuint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform1i(location, data);	
	unbind();
}

void wrench::gl::Shader::uniform(const string name, const float data)
{
	bind();
	GLuint location = glGetUniformLocation(shader_id, name.c_str());
	glUniform1f(location, data);
	unbind();
}

void wrench::gl::Shader::bindAttributeLocation(const string name, GLuint index)
{
    glBindAttribLocation(shader_id, index, name.c_str());
}

char* wrench::gl::Shader::_loadShaderSource(const string &filename)
{
	char* shaderSource;
	
	ifstream shaderFile;
	shaderFile.open(filename.c_str(), ios::in | ios::binary | ios::ate);
	
	if(shaderFile.is_open())
	{
		//	Get the size of the file
		long size = shaderFile.tellg();
		
		//	If there is source read it in
		if (size > 0)
		{
			//	Reset back to the beginning of the file
			shaderFile.seekg(0, ios::beg);
			
			//	Read in all the source
			shaderSource = new char[size + 1];
			shaderFile.read(shaderSource, size);
			shaderSource[size] = '\0';
		}
		//	Dont forget to close the file
		shaderFile.close();
	}
	
	return shaderSource;
}

bool wrench::gl::Shader::_validateShader(GLuint shader, const string &filename = "")
{
	bool validShader = false;
	
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
	validShader = length <= 0;
	
	if (!validShader) 
	{
		cout << "Shader " << shader << " (" << filename << ") compile error: " << buffer << endl;
	}

    return validShader;
}

bool wrench::gl::Shader::_validateProgram(GLuint program)
{
	bool validProgram = false;
	
	const unsigned int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	GLsizei length = 0;
    
	glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
	
	if (length <= 0)
    {
		glValidateProgram(program);
		GLint status;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
		
		validProgram = status != GL_FALSE;
		if (!validProgram)
		{
			cout << "Error validating shader " << program << endl;
		}
    }
	else 
	{
		cout << "Program " << program << " link error: " << buffer << endl;
	}
	
    return validProgram;
}
