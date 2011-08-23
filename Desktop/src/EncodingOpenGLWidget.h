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

#include "AbstractGLContext.h"

class EncodingOpenGLWidget : public QGLWidget
{
  Q_OBJECT

private:
  QColor		m_clearColor;
  QSemaphore	m_encodingLock;

  float m_width;
  float m_height;

public:
  EncodingOpenGLWidget(QWidget *parent);
  EncodingOpenGLWidget(QWidget* parent, AbstractGLContext* glContext, QColor clearColor);
  virtual ~EncodingOpenGLWidget();

  void reinit(float width, float height);
  void initializeGL();
  void updateScene();
  void setGLContext(AbstractGLContext* glContext);
  void cameraSelectMode(int mode);
  void encode();

  AbstractGLContext*    m_glContext;

protected:
  void paintGL();
  void resizeGL(int width, int height);
};

#endif	// _ENCODING_OPEN_GL_WIDGET_H_
