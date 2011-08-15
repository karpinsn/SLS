/**
 *  @file
 *  @author   Nikolaus Karpinsky
 *
 *  Class used to calculate the FPS in an OpenGL application.
 *  frameUpdate(void) should be called for every draw and then
 *  getFrameRate(void) should be called whenever the FPS is required.
 */

#ifndef _FPS_CALCULATOR_H_
#define _FPS_CALCULATOR_H_

#ifdef __APPLE__
#include <glew.h>
#include <OpenGL/gl.h>
#elif _WIN32
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "time.h"

namespace wrench
{
namespace gl
{
namespace utils
{
class FPSCalculator
{
private:
  time_t  m_previousTime; //  Holds the previous time. This is updated every time getFrameRate() is called
  int     m_frameCount;   //  Number of frames inbetween sucessive calls to frameUpdate()

public:
  FPSCalculator(void);

  /**
    * Function which should be called for every draw.
    *
    * This function should be called for every draw in the OpenGL application.
    * It simply updates the count of frames in between sucessive calles to
    * getFrameRate(void)
    */
  void    frameUpdate(void);

  /**
    * Returns the FPS expressed as a double
    *
    * Returns the FPS expressed as a double. This function should be called
    * outside of the draw function and only as a status update. As there are
    * more calls to frameUpdate(void) the better the FPS calculation will be
    * as the frame time will get averaged across multiple frames
    *
    * @return The current frames per second calculation expressed as a double
    */
  double  getFrameRate(void);

};
}
}
}

#endif //_FPS_CALCULATOR_H_
