#include "CaptureController.h"

CaptureController::CaptureController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
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
  cameraGLWidget->updateScene();
}

void CaptureController::init(void)
{
  captureGLWidget->setGLContext(&m_gl3DContext);

  cameraGLWidget->setGLContext(&m_glCameraContext);

  m_glCameraContext.setBuffer(&m_buffer);
  m_camera.init(&m_buffer);
  m_frameCapture.init(&m_buffer);

  _connectSignalsWithController();
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
  CameraConnectDialog dialog;
  lens::Camera *camera = dialog.getCamera();

  m_camera.setCamera(camera);

  //  Reinitalize OpenGL stuff
  captureGLWidget->makeCurrent();
  m_gl3DContext.resizeInput(camera->getWidth(), camera->getHeight());
  cameraGLWidget->makeCurrent();
  m_glCameraContext.resizeInput(camera->getWidth(), camera->getHeight());

  m_camera.start();
}

void CaptureController::disconnectCamera(void)
{
  m_camera.stop();
}

void CaptureController::dropFrame(void)
{
  m_dropFrame = true;
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

    //  Only do this if m_glCameraContex is visible
    if(cameraGLWidget->isVisible())
    {
      cameraGLWidget->makeCurrent();
      m_glCameraContext.newImage(im_gray);
      cameraGLWidget->updateScene();
    }

    captureGLWidget->makeCurrent();
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
  connect(&m_frameCapture, SIGNAL(newFrame(IplImage*)), this, SLOT(newFrame(IplImage*)));
  connect(openCameraButton, SIGNAL(clicked()), this, SLOT(connectCamera()));
  connect(closeCameraButton, SIGNAL(clicked()), this, SLOT(disconnectCamera()));
  connect(calibrateButton, SIGNAL(clicked()), this, SLOT(captureReference()));
  connect(dropFrameButton, SIGNAL(clicked()), this, SLOT(dropFrame()));
}
