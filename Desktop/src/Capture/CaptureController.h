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

#include "Capture\CaptureTypeSelectDialog.h"
#include "Capture\ICapture.h"
#include "Capture\NineFringe\NineFringeCapture.h"
#include "CameraCapture.h"
#include "ImageBuffer.h"
#include "FrameCapture.h"

#define SettingsScalingFactor "CaptureScalingFactor"
#define SettingsGammaValue    "CaptureGammaFactor"

class CaptureController : public QWidget, private Ui::Capture
{
  Q_OBJECT

private:
  shared_ptr<ICapture>		m_gl3DContext;
  shared_ptr<CameraCapture> m_camera;
  shared_ptr<FrameCapture>  m_frameCapture;
  shared_ptr<ImageBuffer>   m_buffer;

  bool                    m_dropFrame;
  QStatusBar*             m_infoBar;
  QLabel                  m_fpsLabel;
  QLabel                  m_3dpsLabel;

  QSettings               m_settings;
  QTimer                  m_frameRateTimer;

public slots:
  void newFrame(IplImage *frame);
  void captureReference(void);
  void connectCamera(void);
  void disconnectCamera(void);
  void dropFrame(void);
  void newGammaValue(double gammaValue);
  void newScalingFactor(double scalingFactor);
  void newViewMode(QString viewMode);
  void updateFPS(void);

public:
    CaptureController(QWidget* parent = 0);
    ~CaptureController();

  void init(void);
  void setInfoBar(QStatusBar* infoBar);
  void cameraSelectMode(int mode);

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *);

private:
    void _connectSignalsWithController(void);
    void _readSettings(void);
};

#endif	// _CAPTURE_CONTROLLER_H_
