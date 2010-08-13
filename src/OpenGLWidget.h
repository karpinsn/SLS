
#ifndef _OPENGLWIDGET_H_
#define _OPENGLWIDGET_H_

#include <glew.h>

#include <QtCore/QTimer>
#include <QtOpenGL/QGLWidget>
#include <QtGui>
#include <iostream>

#include "Holoencoder.h"
#include "Holodecoder.h"
#include "Camera.h"
#include "AbstractGLContext.h"

class OpenGLWidget : public QGLWidget
{
private:
	AbstractGLContext		*m_glContext;
	
	QColor					m_clearColor;
public:
	OpenGLWidget(QWidget* parent, AbstractGLContext* glContext, QColor clearColor);
	~OpenGLWidget();

	void updateScene();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
};

#endif
