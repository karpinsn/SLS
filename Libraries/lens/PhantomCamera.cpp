/*
 *  PhantomCamera.cpp
 */

#include "PhantomCamera.h"

lens::PhantomCamera::PhantomCamera(void) : QThread()
{
	m_cameraImage = NULL;
    m_cameraNumber = 0;
}

void lens::PhantomCamera::init(void)
{
  string workingDirectory("/home/karpinsn/tmp");

  int registrationStatus = PhLVRegisterClientEx(workingDirectory.c_str(), NULL, PHCONHEADERVERSION);

  if(registrationStatus < 0)
  {
    //  Failed to register
  }
  else
  {
    //  Connected
  }
}

void lens::PhantomCamera::open(void)
{
  m_running = true;
  this->start();
}

void lens::PhantomCamera::close(void)
{
  m_running = false;

  PhLVUnregisterClient();
}

float lens::PhantomCamera::getWidth(void)
{
  PACQUIPARAMS pParams = new ACQUIPARAMS;
  PhGetCineParams ( m_CN, 1, pParams, pCineBMI);

  float width = pParams->ImWidth;
  delete pParams;

  return width;
}

float lens::PhantomCamera::getHeight(void)
{
  PACQUIPARAMS pParams = new ACQUIPARAMS;
  PhGetCineParams ( m_CN, 1, pParams, pCineBMI);

  float height = pParams->ImHeight;
  delete pParams;

  return height;
}

std::string lens::PhantomCamera::cameraName(void)
{
    return "Phantom Camera Driver";
}

void lens::PhantomCamera::run()
{
  IMRANGE range;
  range.First = BMP_NO;
  range.Cnt = 1;

  while(m_running)
  {
    PhGetImage(m_cameraNumber, &CINE_CURRENT, &range, GI_INTERPOLATED, m_cameraImage->imageData);
  }
}
