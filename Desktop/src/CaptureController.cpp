#include "CaptureController.h"

CaptureController::CaptureController(QWidget* parent) : QWidget(parent)
{

}

CaptureController::~CaptureController()
{
}

void CaptureController::showEvent(QShowEvent *event)
{
  m_camera.start();

  //m_glCameraContext.setBackBuffer(m_buffer.popFrame());
  //m_glCameraContext.swapBuffers();
  m_cameraTimer.start();
  startTimer(0);

  _updateGL();
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
  
  IplImage *im_gray = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
  cvCvtColor(image,im_gray,CV_RGB2GRAY);
  
  m_gl3DContext.newImage(im_gray);
  
  cvReleaseImage(&im_gray);
  cvReleaseImage(&image);
  _updateGL();
}

void CaptureController::_updateGL(void)
{
  //OpenGLWidget* glContext = findChild<OpenGLWidget*>(QString::fromUtf8("cameraGLWidget"));
  OpenGLWidget *glContext = findChild<OpenGLWidget*>(QString::fromUtf8("captureGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("ViewController - _updateGL: Unable to find OpenGL Widget");
  }
}
