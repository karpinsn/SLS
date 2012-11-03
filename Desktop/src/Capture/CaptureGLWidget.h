/*
 @filename
 @author	Nikolaus Karpinsky
 @since		11/3/12 
 */

#ifndef _CAPTURE_GL_WIDGET_H_
#define _CAPTURE_GL_WIDGET_H_

#ifdef __APPLE__
#include <glew.h>
#include <QtOpenGL/QGLWidget>
#elif _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#else
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#endif

#include <QtCore/QTimer>
#include <QtGui>
#include <iostream>

//	GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//	Wrench includes
#include <wrench/gl/Camera.h>
#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Shader.h>
#include <wrench/gl/Texture.h>
#include <wrench/gl/utils/AxisDisplay.h>
#include <wrench/gl/utils/TextureDisplay.h>

#include "Holoencoder.h"
#include "Holodecoder.h"
#include <wrench/gl/Camera.h>
#include "IGLContext.h"
#include "ICapture.h"

class CaptureGLWidget : public QGLWidget
{
  Q_OBJECT

public:
  enum DisplayMode 
  {
	Geometry,
	GeometryTexture,
	Phase,
	Depth
  };

private:
  QColor					m_clearColor;
  int m_width;
  int m_height;

  ICapture*			  m_captureDecoder;
  Camera			  m_camera;
  Arcball			  m_controller;
  shared_ptr<TriMesh> m_mesh;
  AxisDisplay		  m_axis;

  ShaderProgram		  m_finalRenderColor;
  TextureDisplay	  m_textureDisplay;
  DisplayMode		  m_displayMode;

public:
  CaptureGLWidget(QWidget *parent);

  void initializeGL();
  void resizeCapture(int width, int height);
  void updateScene();
  void setCaptureContext(ICapture* glContext);
  void cameraSelectMode(int mode);
  void setDisplayMode(enum DisplayMode mode);

protected:
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
};

#endif	// _CAPTURE_GL_WIDGET_H_
