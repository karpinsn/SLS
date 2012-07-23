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
#include <windows.h>
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#else
#include <GL/glew.h>
#include <QtOpenGL/QGLWidget>
#endif

#include <QtCore/QTimer>
#include <QSemaphore>
#include <QtGui>
#include <iostream>

#include "IGLContext.h"
#include "IEncodingGLContext.h"
#include "IDecodingGLContext.h"

class EncodingOpenGLWidget : public QGLWidget
{
  Q_OBJECT

private:
  QColor		m_clearColor;
  QSemaphore	m_codecLock;

  bool m_encode;
  bool m_decode;

  float m_width;
  float m_height;

  IEncodingGLContext* m_encodingContext;
  IDecodingGLContext* m_decodingContext;

public:
  EncodingOpenGLWidget(QWidget *parent);
  EncodingOpenGLWidget(QWidget* parent, IGLContext* glContext, QColor clearColor);
  virtual ~EncodingOpenGLWidget();

  void reinit(float width, float height);
  void initializeGL();
  void updateScene();
  void setGLContext(IGLContext* glContext);
  void setEncodingContext(IEncodingGLContext* encodingContext);
  void setDecodingContext(IDecodingGLContext* decodingContext);
  void cameraSelectMode(int mode);
  MeshInterchange* decode();
  MeshInterchange* encode();

  IGLContext*    m_glContext;

protected:
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent* event);
};

#endif	// _ENCODING_OPEN_GL_WIDGET_H_
