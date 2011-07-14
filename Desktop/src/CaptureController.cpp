#include "CaptureController.h"

CaptureController::CaptureController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);                    //  Creates the UI objects
  _connectSignalsWithController();  //  Connects the UI objects to the slots in the controller
  _readSettings();                  //  Reads the persisted settings

  m_dropFrame = false;
}

CaptureController::~CaptureController()
{
}

void CaptureController::showEvent(QShowEvent *event)
{
  //  Connect to camera
  m_frameCapture.start();

  captureGLWidget->updateScene();
}

void CaptureController::init(void)
{
  captureGLWidget->setGLContext(&m_gl3DContext);

  m_camera.init(&m_buffer);
  m_frameCapture.init(&m_buffer);
}

void CaptureController::setInfoBar(QStatusBar* infoBar)
{
  m_infoBar = infoBar;
}

void CaptureController::captureReference(void)
{
  m_gl3DContext.captureReferencePlane();
}

void CaptureController::cameraSelectMode(int mode)
{
  m_gl3DContext.cameraSelectMode(mode);
}

void CaptureController::connectCamera(void)
{
  m_infoBar->showMessage("Connecting to camera...");

  CameraConnectDialog dialog;
  lens::Camera *camera = dialog.getCamera();

  m_camera.setCamera(camera);

  //  Reinitalize OpenGL stuff
  m_gl3DContext.resizeInput(camera->getWidth(), camera->getHeight());

  m_camera.start();
  m_infoBar->showMessage("Connected to the camera");
}

void CaptureController::disconnectCamera(void)
{
  m_camera.stop();
  m_infoBar->showMessage("Camera stopped");
}

void CaptureController::dropFrame(void)
{
  m_dropFrame = true;
}

void CaptureController::newGammaValue(double gammaValue)
{
  //  Set the new gamma value
  m_gl3DContext.setGammaCutoff(gammaValue);

  //  Persist the new gamma value
  m_settings.setValue(SettingsGammaValue, gammaValue);
}

void CaptureController::newScalingFactor(double scalingFactor)
{
  m_gl3DContext.setScalingFactor(scalingFactor);

  //  Persist the new scaling factor
  m_settings.setValue(SettingsScalingFactor, scalingFactor);
}

void CaptureController::newFrame(IplImage *frame)
{
  if(!m_dropFrame)  //  If we dont drop a frame then process it
  {
    IplImage *im_gray = frame;
    bool releaseGray = false;

    if(frame->nChannels > 1)
    {
      im_gray = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
      cvCvtColor(frame, im_gray, CV_RGB2GRAY);
      releaseGray = true;
    }

    m_gl3DContext.newImage(im_gray);

    if(releaseGray)
    {
      cvReleaseImage(&im_gray);
    }

    cvReleaseImage(&frame);
    captureGLWidget->updateScene();
  }
  else  // Drop a frame
  {
    m_dropFrame = false;  //  We only want to drop one frame so set it back
  }
}

void CaptureController::_connectSignalsWithController(void)
{
  connect(&m_frameCapture,    SIGNAL(newFrame(IplImage*)),  this, SLOT(newFrame(IplImage*)));
  connect(openCameraButton,   SIGNAL(clicked()),            this, SLOT(connectCamera()));
  connect(closeCameraButton,  SIGNAL(clicked()),            this, SLOT(disconnectCamera()));
  connect(calibrateButton,    SIGNAL(clicked()),            this, SLOT(captureReference()));
  connect(dropFrameButton,    SIGNAL(clicked()),            this, SLOT(dropFrame()));
  connect(gammaBox,           SIGNAL(valueChanged(double)), this, SLOT(newGammaValue(double)));
  connect(scalingFactorBox,   SIGNAL(valueChanged(double)), this, SLOT(newScalingFactor(double)));
}

void CaptureController::_readSettings(void)
{
  //  Read in the settings file and set the settings
  gammaBox->setValue(         m_settings.value(QString("CaptureGammaValue"),    .30).toDouble());
  scalingFactorBox->setValue( m_settings.value(QString("CaptureScalingFactor"), .04).toDouble());
}
