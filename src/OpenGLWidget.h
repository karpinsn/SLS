
#ifndef _OPENGLWIDGET_H_
#define _OPENGLWIDGET_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <QtOpenGL/QGLWidget>
#else
	#include <GL/glew.h>
	#include <windows.h>
	#include <QtOpenGL/QGLWidget>
#endif

#include <QtCore/QTimer>
#include <QtGui>
#include <iostream>

#include "Holoencoder.h"
#include "Holodecoder.h"
#include "Camera.h"
#include "AbstractGLContext.h"

class Holodecoder;

class OpenGLWidget : public QGLWidget
{
private:
	AbstractGLContext		*m_glContext;
	
	QColor					m_clearColor;
	
	QTime movieTimer;
	ImageIO m_aviIO;
	Holodecoder* m_holoDecoder;
	
public:
	OpenGLWidget(QWidget* parent, AbstractGLContext* glContext, QColor clearColor);
	~OpenGLWidget();

	void updateScene();
	void setNewGLContext(AbstractGLContext* glContext);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
	void playMovie(string movieFile, Holodecoder* decoder);
	
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
	
	void timerEvent(QTimerEvent* event);
};

#endif
