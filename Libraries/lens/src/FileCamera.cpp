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
  currentImage = 0;
}

lens::FileCamera::~FileCamera()
{

}

void lens::FileCamera::init(void)
{
  m_images[0] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe1.png");
  m_images[1] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe2.png");
  m_images[2] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe3.png");
  m_images[3] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe4.png");
  m_images[4] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe5.png");
  m_images[5] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe6.png");
  m_images[6] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe7.png");
  m_images[7] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe8.png");
  m_images[8] = _readImage("/home/karpinsn/Dropbox/Documents/Grad School/Deere Particle Track/MultiwavelengthPatters/fringe9.png");

  /*
  m_images[0] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O006.bmp");
  m_images[1] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O007.bmp");
  m_images[2] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O008.bmp");
  m_images[3] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O012.bmp");
  m_images[4] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O013.bmp");
  m_images[5] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O014.bmp");
  m_images[6] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O009.bmp");
  m_images[7] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O010.bmp");
  m_images[8] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/O011.bmp");
  */
/*
  m_images[9] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref1.bmp");
  m_images[10] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref2.bmp");
  m_images[11] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref3.bmp");
  m_images[12] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref7.bmp");
  m_images[13] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref8.bmp");
  m_images[14] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref9.bmp");
  m_images[15] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref4.bmp");
  m_images[16] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref5.bmp");
  m_images[17] = _readImage("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/Reference/ref6.bmp");
  */
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
  return m_images[0]->width;
}

float lens::FileCamera::getHeight(void)
{
  return m_images[0]->height;
}

std::string lens::FileCamera::cameraName(void)
{
  return "File Based Camera Driver";
}

void lens::FileCamera::run()
{
  while(m_running)
  {
    notifyObservers(m_images[currentImage]);

    currentImage = ++currentImage % 9;

    msleep(33.3);
  }
}

IplImage* lens::FileCamera::_readImage(const string &filename)
{
    IplImage* image = cvLoadImage(filename.c_str());

    cvCvtColor(image, image, CV_BGR2RGB);

    return image;
}
