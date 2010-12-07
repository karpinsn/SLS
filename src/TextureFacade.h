/*
 Filename:		TextureFacade.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	12/07/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 12/07/10 - Nik Karpinsky - Added methods for transfer to and from using DMA
 */

#ifndef _TEXTURE_FACADE_H_
#define _TEXTURE_FACADE_H_

#include <QtOpenGL/QGLWidget>

#include "OGLStatus.h"
#include <cv.h>

class TextureFacade
{
private:	
	GLuint	m_width;			//	Width of the texture
	GLuint	m_height;			//	Height of the texture
	
	GLuint	m_textureId;		//	Texture identifier
	GLuint	m_PBOId;			//	PBO identifier. Used for fast DMA transfers
	int		m_dataSize;			//	Size of the data. i.e. sizeof(m_dataType...)
	
	//	Consult glTexImage2D for what these variables do
	GLint	m_internalFormat;	//	Internal format of the texture to render to
	GLenum	m_format;			//	Format of the texture
	GLenum	m_dataType;			//	Data type of the texture
public:
	TextureFacade(void);
	~TextureFacade();
	
	bool			init(GLuint width, GLuint height, GLint internalFormat = GL_RGBA32F_ARB, GLenum format = GL_RGBA, GLenum dataType = GL_FLOAT);
	void			bind();
	void			unbind();
	
	const	GLuint	getTextureId(void)							const;
	const	GLenum	getTextureTarget(void)						const;
	const	int		getChannelCount(void)						const;
	const	GLuint	getWidth(void)								const;
	const	GLuint	getHeight(void)								const;
	const	GLuint	getFormat(void)								const;
	const	GLuint	getDataType(void)							const;
	
			bool	transferFromTexture(IplImage* image);
			bool	transferToTexture(const IplImage* image);
	
private:	
			void	_transferFloatData(const char* source, char* dest, int sourceNChannels, int destNChannels, int sourceWidthStep, int destWidthStep);
			void	_transferByteData(const char* source, char* dest, int sourceNChannels, int destNChannels, int sourceWidthStep, int destWidthStep);
			bool	_checkImageCompatibility(const IplImage* image) const;
};

#endif	// _TEXTURE_FACADE_H_
