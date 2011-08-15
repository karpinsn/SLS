/**
  * @file
  * @author	Nikolaus Karpinsky
  * @date      09/01/09
  *
  * Class to convert OpenGL enum types to useful data sizes.
  */

#ifndef _CONVERTER_H_
#define _CONVERTER_H_

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

#include "../Logger.h"

namespace wrench
{
namespace gl
{
class Converter
{
public:
  /**
    * Static method which takes an OpenGL size enum and returns its proper sizeof() size.
    *
    * @param type - OpenGL data type enum
    *
    * @return sizeof() size of the data type referenced by the OpenGL type enum
    */
  static size_t typeToSize (const GLenum type);
};
}
}

#endif	// _CONVERTER_H_
