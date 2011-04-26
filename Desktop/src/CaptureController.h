/*
 Filename:		CaptureController.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
 */

#ifndef _CAPTURE_CONTROLLER_H_
#define _CAPTURE_CONTROLLER_H_

#include <QWidget>

#include "OpenGLWidget.h"
#include "MultiWavelengthCapture.h"

class CaptureController : public QWidget
{
  Q_OBJECT

private:
  MultiWavelengthCapture m_glContext;

public:
    CaptureController(QWidget* parent = 0);
    ~CaptureController();

    void init(void);

private:
    void _updateGL(void);
};

#endif	// _CAPTURE_CONTROLLER_H_
