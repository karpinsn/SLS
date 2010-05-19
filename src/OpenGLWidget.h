
#ifndef _OPENGLWIDGET_H_
#define _OPENGLWIDGET_H_

#include <glew.h>

#include <QtCore/QTimer>
#include <QtOpenGL/QGLWidget>
#include <QtGui>
#include <iostream>

#include "AOpenGLContext.h"
#include "Camera.h"

class OpenGLWidget : public QGLWidget
{
private:
	AOpenGLContext* m_glContext;
	
	QColor			m_clearColor;
public:
	OpenGLWidget(QWidget* parent, AOpenGLContext* glContext, QColor clearColor);
	~OpenGLWidget();

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
