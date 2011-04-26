#include "CaptureController.h"

CaptureController::CaptureController(QWidget* parent) : QWidget(parent)
{

}

CaptureController::~CaptureController()
{
}

void CaptureController::init(void)
{
  OpenGLWidget *glWidget = findChild<OpenGLWidget*>(QString::fromUtf8("cameraGLWidget"));
  glWidget->setGLContext(&m_glContext);
}

void CaptureController::_updateGL(void)
{
  Logger::logDebug("ViewController - _updateGL: Enter");

  OpenGLWidget* glContext = findChild<OpenGLWidget*>(QString::fromUtf8("encoderGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("ViewController - _updateGL: Unable to find OpenGL Widget");
  }
}
