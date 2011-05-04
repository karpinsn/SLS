/*
 Filename:		OpenGLWidget.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _OPEN_GL_WIDGET_H_
#define _OPEN_GL_WIDGET_H_

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
#include <QtGui>
#include <iostream>

#include "Holoencoder.h"
#include "Holodecoder.h"
#include <wrench/gl/Camera.h>
#include "AbstractGLContext.h"

class Holodecoder;

class OpenGLWidget : public QGLWidget
{
  Q_OBJECT

private:
  QColor					m_clearColor;

public:
  OpenGLWidget(QWidget *parent);
  OpenGLWidget(QWidget* parent, AbstractGLContext* glContext, QColor clearColor);
  virtual ~OpenGLWidget();

  void initializeGL();
  void updateScene();
  void setGLContext(AbstractGLContext* glContext);
  void cameraSelectMode(int mode);

  AbstractGLContext*    m_glContext;
  Holodecoder*          m_holoDecoder;

protected:
  void paintGL();
  void resizeGL(int width, int height);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent* event);
};

#endif	// _OPEN_GL_WIDGET_H_