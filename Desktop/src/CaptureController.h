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
#include "CameraGLContext.h"
#include "CameraCapture.h"
#include "ImageBuffer.h"

class CaptureController : public QWidget
{
  Q_OBJECT

private:
  MultiWavelengthCapture  m_gl3DContext;
  CameraGLContext         m_glCameraContext;
  CameraCapture           m_camera;
  ImageBuffer             m_buffer;
  QTime                   m_cameraTimer;

public:
    CaptureController(QWidget* parent = 0);
    ~CaptureController();

    void init(void);

protected:
    virtual void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent* event);

private:
    void _update3DGL(void);
    void _updateCameraGL(void);
};

#endif	// _CAPTURE_CONTROLLER_H_
