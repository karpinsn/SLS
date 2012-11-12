/*
 Filename:		OpenGLWidget.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _ENCODING_OPEN_GL_WIDGET_H_
#define _ENCODING_OPEN_GL_WIDGET_H_

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

#include <wrench/gl/utils/TextureDisplay.h>

#include <QtCore/QTimer>
#include <QSemaphore>
#include <QtGui>
#include <iostream>

#include "IGLContext.h"
#include "IEncoder.h"
#include "IDecoder.h"

class EncodingOpenGLWidget : public QGLWidget
{
  Q_OBJECT

private:
  QColor		m_clearColor;
  shared_ptr<QSemaphore>	m_codecLock;

  wrench::gl::utils::TextureDisplay	  m_textureDisplay;

  bool m_encode;
  bool m_decode;
  bool m_preview;

  float m_width;
  float m_height;

  IEncoder* m_encodingContext;
  IDecoder* m_decodingContext;

public:
  EncodingOpenGLWidget(QWidget *parent);

  void reinit(float width, float height);
  void initializeGL();
  void updateScene();
  void setEncodingContext(IEncoder* encodingContext);
  void setDecodingContext(IDecoder* decodingContext);
  void cameraSelectMode(int mode);
  MeshInterchange* decode();
  MeshInterchange* encode();
  void previewDecode();
  void previewEncode();

protected:
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
};

#endif	// _ENCODING_OPEN_GL_WIDGET_H_
