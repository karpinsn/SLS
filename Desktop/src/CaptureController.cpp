#include "CaptureController.h"

CaptureController::CaptureController(QWidget* parent) : QWidget(parent)
{
}

CaptureController::~CaptureController()
{
}

void CaptureController::showEvent(QShowEvent *event)
{
  //  Connect to camera
  m_frameCapture.start();
  connectToCamera();

  //  Reinitalize OpenGL stuff

  _update3DGL();
  _updateCameraGL();
}

void CaptureController::init(void)
{
  OpenGLWidget *captureWidget = findChild<OpenGLWidget*>(QString::fromUtf8("captureGLWidget"));
  captureWidget->setGLContext(&m_gl3DContext);

  OpenGLWidget *cameraWidget = findChild<OpenGLWidget*>(QString::fromUtf8("cameraGLWidget"));
  cameraWidget->setGLContext(&m_glCameraContext);

  m_glCameraContext.setBuffer(&m_buffer);
  m_camera.init(&m_buffer);
  m_frameCapture.init(&m_buffer);

  _connectSignalsWithController();
}

void CaptureController::captureReference(void)
{
  m_gl3DContext.captureReferencePlane();
}

void CaptureController::connectToCamera(void)
{
  CameraConnectDialog dialog;
  lens::Camera *camera = dialog.getCamera();
  //  We got a valid camera. Start it up
  m_camera.setCamera(camera);
  m_camera.start();
}

void CaptureController::newFrame(IplImage *frame)
{

  //  Only do this if m_glCameraContex is visible
  OpenGLWidget* cameraContext = findChild<OpenGLWidget*>(QString::fromUtf8("cameraGLWidget"));
  if(cameraContext->isVisible())
  {
    cameraContext->makeCurrent();
    m_glCameraContext.newImage(frame);
    _updateCameraGL();
  }

  OpenGLWidget *captureContext = findChild<OpenGLWidget*>(QString::fromUtf8("captureGLWidget"));
  captureContext->makeCurrent();

  IplImage *im_gray = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
  cvCvtColor(frame, im_gray, CV_RGB2GRAY);

  m_gl3DContext.newImage(im_gray);

  cvReleaseImage(&im_gray);
  cvReleaseImage(&frame);
  _update3DGL();
}

void CaptureController::_update3DGL(void)
{
  OpenGLWidget *glContext = findChild<OpenGLWidget*>(QString::fromUtf8("captureGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("ViewController - _update3DGL: Unable to find 3D Decoding OpenGL Widget");
  }
}

void CaptureController::_updateCameraGL(void)
{
  OpenGLWidget* glContext = findChild<OpenGLWidget*>(QString::fromUtf8("cameraGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("ViewController - _updateGL: Unable to find Camera view OpenGL Widget");
  }
}

void CaptureController::_connectSignalsWithController(void)
{
  connect(&m_frameCapture, SIGNAL(newFrame(IplImage*)), this, SLOT(newFrame(IplImage*)));
}
