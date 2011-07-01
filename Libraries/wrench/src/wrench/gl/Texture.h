/*
 Filename:		TextureFacade.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	12/07/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 12/07/10 - Nik Karpinsky - Added methods for transfer to and from using DMA
 */

#ifndef _WRENCH_GL_TEXTURE_H_
#define _WRENCH_GL_TEXTURE_H_

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

#include "Converter.h"
#include "OGLStatus.h"
#include <cv.h>

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

namespace wrench
{
namespace gl
{
class Texture
{
private:
  GLuint	m_width;			//	Width of the texture
  GLuint	m_height;			//	Height of the texture

  int		m_dataSize;			//	Size of the data. i.e. sizeof(m_dataType...)

  //	Consult glTexImage2D for what these variables do
  GLint	m_internalFormat;	//	Internal format of the texture to render to
  GLenum	m_format;			//	Format of the texture
  GLenum	m_dataType;			//	Data type of the texture

#ifdef USE_VRJ
  vrj::opengl::ContextData<GLuint> vrjTextureHandle;
  vrj::opengl::ContextData<GLuint> vrjPBOHandle;
#define m_textureId (*vrjTextureHandle)
#define m_PBOId     (*vrjPBOHandle)
#else
  GLuint m_textureId;		// Texture identifier
  GLuint m_PBOId;			// PBO identifier. Used for fast DMA transfers
#endif

public:
  Texture(void);
  ~Texture();

  bool		init(const GLuint width, const GLuint height, GLint internalFormat = GL_RGBA32F_ARB, GLenum format = GL_RGBA, GLenum dataType = GL_FLOAT);
  bool      reinit(const GLuint width, const GLuint height, GLint internalFormat = GL_RGBA32F_ARB, GLenum format = GL_RGBA, GLenum dataType = GL_FLOAT);
  void      bind();
  void      bind(GLenum texture);
  void		unbind();

  const	GLuint	getTextureId(void)      const;
  const	GLenum	getTextureTarget(void)	const;
  const	int     getChannelCount(void)	const;
  const	GLuint	getWidth(void)          const;
  const	GLuint	getHeight(void)         const;
  const	GLuint	getFormat(void)         const;
  const	GLuint	getDataType(void)       const;

  bool	transferFromTexture(IplImage* image);
  bool	transferToTexture(const IplImage* image);
  bool  transferChannelToTexture(const IplImage* image, int channelNumber);

private:
  void	_transferFloatData(const char* source, char* dest, int sourceNChannels, int destNChannels, int sourceWidthStep, int destWidthStep);
  void	_transferByteData(const char* source, char* dest, int sourceNChannels, int destNChannels, int sourceWidthStep, int destWidthStep);
  bool	_checkImageCompatibility(const IplImage* image) const;
};
}
}

#endif	// _WRENCH_GL_TEXTURE_H_
