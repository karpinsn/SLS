#include "FPSCalculator.h"

wrench::gl::utils::FPSCalculator::FPSCalculator(void)
{
  m_previousTime = time(NULL);
  m_frameCount = 0;
}

void wrench::gl::utils::FPSCalculator::frameUpdate(void)
{
  ++m_frameCount;
}

double wrench::gl::utils::FPSCalculator::getFrameRate(void)
{
  time_t currentTime = time(NULL);
  double timeInterval = difftime(currentTime, m_previousTime);

  float framesPerSecond = m_frameCount / timeInterval;
  m_previousTime = currentTime;
  m_frameCount = 0;

  return framesPerSecond;
}
