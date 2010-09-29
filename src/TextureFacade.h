/*
 *  TextureFacade.h
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#ifndef _TEXTURE_FACADE_H_
#define _TEXTURE_FACADE_H_

#include <QtOpenGL/QGLWidget>

#include "OGLStatus.h"

class TextureFacade
{
private:
	GLuint m_width;			//	Width of the texture
	GLuint m_height;		//	Height of the texture
	
	GLuint m_textureId;		//	Texture identifier
	GLuint m_PBOId;			//	PBO identifier. Used for fast DMA transfers
	
	//	Consult glTexImage2D for what these variables do
	GLint m_internalFormat;	//	Internal format of the texture to render to
	GLenum m_format;		//	Format of the texture
	GLenum m_dataType;		//	Data type of the texture
public:
	TextureFacade(void);
	~TextureFacade();
	
	bool init(GLuint width, GLuint height, GLint internalFormat = GL_RGBA32F_ARB, GLenum format = GL_RGBA, GLenum dataType = GL_FLOAT);
	void bind();
	void unbind();
	
	const GLuint getTextureId(void) const;
	const GLenum getTextureTarget(void) const;
	
	void transferToTexture(const void* data);
	void transferFromTexture(void* data);
	//void* getDataPointer();						//	Caution should be used when using this method
};

#endif