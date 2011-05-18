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

  m_camera.start();
  m_cameraTimer.start();
  startTimer(0);

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
}

void CaptureController::timerEvent(QTimerEvent* event)
{
  m_cameraTimer.restart();
 
  IplImage* image = m_buffer.popFrame();
  
  //  Only do this if m_glCameraContex is visible
  OpenGLWidget* cameraContext = findChild<OpenGLWidget*>(QString::fromUtf8("cameraGLWidget"));
  if(cameraContext->isVisible())
  {
    cameraContext->makeCurrent();
    m_glCameraContext.newImage(image);
    _updateCameraGL();
  }
  OpenGLWidget *captureContext = findChild<OpenGLWidget*>(QString::fromUtf8("captureGLWidget"));
  captureContext->makeCurrent();

  IplImage *im_gray = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
  cvCvtColor(image,im_gray,CV_RGB2GRAY);
  
  m_gl3DContext.newImage(im_gray);
  
  cvReleaseImage(&im_gray);
  cvReleaseImage(&image);
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
