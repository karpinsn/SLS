/*
 Filename:		TextureFacade.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _TEXTURE_FACADE_H_
#define _TEXTURE_FACADE_H_

#include <QtOpenGL/QGLWidget>

#include "OGLStatus.h"
#include <cv.h>

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
	const int getChannelCount(void) const;
	
	void transferFromTexture(void* data);
	//void* getDataPointer();						//	Caution should be used when using this method
	bool transferToTexture(const IplImage* image);
	
private:	
	void _transferFloatDataToTexture(const IplImage* image, void* gpuMem, int channelCount);
	void _transferByteDataToTexture(const IplImage* image, void* gpuMem, int channelCount);
	bool _checkImageCompatibility(const IplImage* image);
};

#endif	// _TEXTURE_FACADE_H_
