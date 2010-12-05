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

const int TextureFacade::getChannelCount(void) const
{
	int channelCount = 1;
	
	if(m_format == GL_RGB || m_format == GL_BGR)
	{
		channelCount = 3;
	}
	else if(m_format == GL_RGBA || m_format == GL_BGRA)
	{
		channelCount = 4;
	}
	
	return channelCount;
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
 
bool TextureFacade::transferToTexture(const IplImage* image)
{
	bool compatible = _checkImageCompatibility(image);
	
	if(compatible)
	{
		const int channelCount = getChannelCount();
		size_t dataSize = m_width * m_height * sizeof(uchar) * channelCount;
		
		bind();
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, m_PBOId);
		glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, dataSize, NULL, GL_STREAM_DRAW);
		GLubyte* gpuMem = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
		
		if(m_dataType == GL_FLOAT)
		{
			_transferFloatDataToTexture(image, gpuMem, channelCount);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, m_format, m_dataType, 0);
			glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		}
		else if(m_dataType == GL_UNSIGNED_BYTE)
		{		
			_transferByteDataToTexture(image, gpuMem, channelCount);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, m_format, m_dataType, 0);
			glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		}	
	}
	return compatible;
}
 
void TextureFacade::_transferFloatDataToTexture(const IplImage* image, void* gpuMem, int channelCount)
{ 
	for(unsigned int y = 0; y < m_height; ++y)
	{
		for(unsigned int x = 0; x < m_width; ++x)
		{
			float* imagePointer = (float*)(image->imageData + image->widthStep*y);
			float* gpuPointer = (float*)(((float*)gpuMem) + image->width*channelCount*y);
 
			for(int channel = 0; channel < channelCount && channel < image->nChannels; channel++)
			{
				gpuPointer[x*channelCount+channel] = imagePointer[x*image->nChannels+channel];
			}
		}
	}
}
 
void TextureFacade::_transferByteDataToTexture(const IplImage* image, void* gpuMem, int channelCount)
 { 
	for(unsigned int y = 0; y < m_height; ++y)
	{
		for(unsigned int x = 0; x < m_width; ++x)
		{
			uchar* imagePointer = (uchar*)(image->imageData + image->widthStep*y);
			uchar* gpuPointer = (uchar*)(((uchar*)gpuMem) + image->width*channelCount*y);
 
			for(int channel = 0; channel < channelCount && channel < image->nChannels; channel++)
			{
				gpuPointer[x*channelCount+channel] = imagePointer[x*image->nChannels+channel];
			}
		}
	}
 }

bool TextureFacade::_checkImageCompatibility(const IplImage* image)
{
	bool compatible = false;
	
	if(image->width		== m_width && 
	   image->height	== m_height && 
	   image->nChannels <= getChannelCount())
	{
		compatible = true;
	}
	
	return compatible;
}