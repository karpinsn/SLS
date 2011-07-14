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

//	Qt auto generated headers
#include "ui_Capture.h"

#include <QWidget>

#include "OpenGLWidget.h"
#include "CameraConnectDialog.h"

#include "MultiWavelengthCapture.h"
#include "CameraGLContext.h"
#include "CameraCapture.h"
#include "ImageBuffer.h"
#include "FrameCapture.h"


#define SettingsScalingFactor "CaptureScalingFactor"
#define SettingsGammaValue    "CaptureGammaFactor"

class CaptureController : public QWidget, private Ui::Capture
{
  Q_OBJECT

private:
  MultiWavelengthCapture  m_gl3DContext;
  CameraGLContext         m_glCameraContext;
  CameraCapture           m_camera;
  FrameCapture            m_frameCapture;
  ImageBuffer             m_buffer;

  bool                    m_dropFrame;
  QStatusBar*             m_infoBar;

  QSettings               m_settings;

public slots:
  void newFrame(IplImage *frame);
  void captureReference(void);
  void connectCamera(void);
  void disconnectCamera(void);
  void dropFrame(void);
  void newGammaValue(double gammaValue);
  void newScalingFactor(double scalingFactor);

public:
    CaptureController(QWidget* parent = 0);
    ~CaptureController();

  void init(void);
  void setInfoBar(QStatusBar* infoBar);
  void cameraSelectMode(int mode);

protected:
    virtual void showEvent(QShowEvent *event);

private:
    void _connectSignalsWithController(void);
    void _readSettings(void);
};

#endif	// _CAPTURE_CONTROLLER_H_
