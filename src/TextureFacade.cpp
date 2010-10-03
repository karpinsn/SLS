/*
 *  TextureFacade.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "TextureFacade.h"

TextureFacade::TextureFacade()
{
	m_width = 0;
	m_height = 0;
}

TextureFacade::~TextureFacade()
{
	glDeleteTextures(1, &m_textureId);
	glDeleteBuffers(1, &m_PBOId);
}

bool TextureFacade::init(const GLuint width, const GLuint height, const GLint internalFormat, const GLenum format, const GLenum dataType)
{
	m_width = width;
	m_height = height;
	m_internalFormat = internalFormat;
	m_format = format;
	m_dataType = dataType;
	
	//	Initialize the PBO
	glGenBuffers(1, &m_PBOId);
	
	//	Initialize the Texture
	//	Generate a texture and bind it
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	
	//	Turn off filtering and wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	//	Allocate memory for the texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, 0);
	
	OGLStatus::logOGLErrors("TextureFacade - init()");
	return true;
}

void TextureFacade::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void TextureFacade::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

const GLuint TextureFacade::getTextureId(void) const
{
	return m_textureId;
}

const GLenum TextureFacade::getTextureTarget(void) const
{
	return GL_TEXTURE_2D;
}

void TextureFacade::transferToTexture(const void* data) 
{
	size_t dataSize = m_width * m_height * sizeof(uchar) * 4;
    
	bind();
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, m_PBOId);
	glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, NULL, GL_STREAM_DRAW);
	
	
	GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	
	if(ptr)
	{
		memcpy(ptr, data, dataSize);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, GL_BGRA, GL_UNSIGNED_BYTE, 0);
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}
	
	OGLStatus::logOGLErrors("TextureFacade - transferToTexture()");
}

void TextureFacade::transferFromTexture(void* data)
{
	size_t dataSize = m_width * m_height * sizeof(float) * 4;
	
	bind();
	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, m_PBOId);
	glBufferData(GL_PIXEL_PACK_BUFFER_ARB, dataSize, NULL, GL_STREAM_READ);
	glReadPixels (0, 0, m_width, m_height, GL_RGBA, GL_FLOAT, 0);
	
	void* mem = glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);   
	memcpy(data, mem, dataSize);
	
	glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0); 
	
	OGLStatus::logOGLErrors("TextureFacade - transferToTexture()");
}