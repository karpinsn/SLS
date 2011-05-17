/*
 Filename:	CameraGLContext.h
 Author:	Nikolaus Karpinsky
 Date Created:	01/30/11
 Last Edited:	01/30/11
 
 Revision Log:
 01/30/11 - Nik Karpinsky - Original creation.
 */
#ifndef _CAMERA_GL_CONTEXT_H_
#define _CAMERA_GL_CONTEXT_H_

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

#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Shader.h>
#include <wrench/gl/Texture.h>
#include <wrench/gl/VBO.h>
#include <wrench/gl/VAO.h>
#include <wrench/gl/Camera.h>

#include "AbstractGLContext.h"

#include "ImageBuffer.h"

using namespace wrench;
using namespace wrench::gl;

class CameraGLContext : public AbstractGLContext
{
private:
  Camera m_camera;
  ImageBuffer *m_buffer;

  ShaderProgram m_textureDisplay;
  Texture m_frame0;
  Texture m_frame1;
  Texture* m_textureBuffer[2];
  int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds

  VAO m_screen;
  VBO m_vertex;
  VBO m_texCoords;

public:
  CameraGLContext(void);

  void setBuffer(ImageBuffer *buffer);
  virtual void init(void);
  virtual void draw(void);
  virtual void resize(int width, int height);
  virtual void cameraSelectMode(int mode);
  virtual void mousePressEvent(int mouseX, int mouseY);
  virtual void mouseMoveEvent(int mouseX, int mouseY);

  void newImage(IplImage* image);
  void swapBuffers(void);
private:
  void _initShaders(void);
  void _initTextures(GLuint width, GLuint height);
  void _cacheQuad(void);
};

#endif	// _CAMERA_GL_CONTEXT_H_
