/*
 *  PhantomCamera.cpp
 */

#include "PhantomCamera.h"

lens::PhantomCamera::PhantomCamera(void) : QThread()
{
	m_CineBMI = (PBITMAPINFO)&CineBMI;
	m_params = new ACQUIPARAMS;
	m_cameraImage = NULL;
    m_cameraNumber = 0;
}

lens::PhantomCamera::~PhantomCamera()
{
	delete m_params;
}

void lens::PhantomCamera::init(void)
{
  int registrationStatus = PhLVRegisterClientEx("/home/karpinsn/tmp", NULL, PHCONHEADERVERSION);

  if(registrationStatus < 0)
  {
    //  Failed to register
	std::cout << "Failed to register with Phantom camera" << std::endl;
  }
  else
  {
    //  Connected
	  std::cout << "Registered with the Phantom Camera" << std::endl;
  }

  float width = getWidth();
  float height = getHeight();
  m_cameraImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
  setFrameRate(36);
  setTrigger(SYNC_EXTERNAL);
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
  PhGetCineParams ( m_cameraNumber, CINE_DEFAULT, m_params, m_CineBMI);
 
  return m_params->ImWidth;
}

float lens::PhantomCamera::getHeight(void)
{
  PhGetCineParams ( m_cameraNumber, CINE_DEFAULT, m_params, m_CineBMI);
 
  return m_params->ImHeight;
}

void lens::PhantomCamera::setFrameRate(int fps)
{
  PhGetCineParams ( m_cameraNumber, CINE_DEFAULT, m_params, m_CineBMI);
  m_params->FrameRate = fps;	//	Set the framerate
  PhSetCineParams( m_cameraNumber, CINE_DEFAULT, m_params);
}

void lens::PhantomCamera::setTrigger(unsigned int triggerMode)
{
  PhGetCineParams ( m_cameraNumber, CINE_DEFAULT, m_params, m_CineBMI);
  m_params->SyncImaging = triggerMode;	//	Set the trigger mode
  PhSetCineParams( m_cameraNumber, CINE_DEFAULT, m_params);
}

std::string lens::PhantomCamera::cameraName(void)
{
    return "Phantom Camera Driver";
}

void lens::PhantomCamera::run()
{
  IMRANGE range;
  range.First = 0;
  range.Cnt = 1;

  while(m_running)
  {
	int cineNumber = CINE_CURRENT;
    PhGetImage(m_cameraNumber, &cineNumber, &range, 0, (PBYTE)m_cameraImage->imageData);
	notifyObservers(m_cameraImage);
  }
}
