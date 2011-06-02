/*
 *  FileCamera.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "FileCamera.h"

lens::FileCamera::FileCamera() : QThread()
{

}

lens::FileCamera::~FileCamera()
{

}

void lens::FileCamera::init(void)
{

}

void lens::FileCamera::open(void)
{
  m_running = true;
  this->start();
}

void lens::FileCamera::close(void)
{
  m_running = false;
}

float lens::FileCamera::getWidth(void)
{
  return 1.0;
}

float lens::FileCamera::getHeight(void)
{
  return 1.0;
}

std::string lens::FileCamera::cameraName(void)
{
        return "File Based Camera Driver";
}

void lens::FileCamera::run()
{
  while(m_running)
  {
    sleep(1000);
  }
}
