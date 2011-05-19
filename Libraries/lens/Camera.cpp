/*
 *  Camera.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 */

#include "Camera.h"


void lens::Camera::addObserver(CameraObserver *observer)
{
  m_observers.push_back(observer);
}

void lens::Camera::notifyObservers(IplImage *frame)
{
  std::vector<CameraObserver*>::iterator iterator;

  for(iterator = m_observers.begin(); iterator < m_observers.end(); iterator++)
  {
    (*iterator)->newFrame(frame);
  }
}
