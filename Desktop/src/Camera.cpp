#include "Camera.h"

Camera::Camera(int deviceNumber) : QThread()
{	
  m_capture = cvCaptureFromCAM(deviceNumber);
}

Camera::~Camera()
{
}

bool Camera::isConnected()
{
  return false;
}

void Camera::run()
{
  while(true)
  {

  }
}
