/**
 @file		EncodingGLContext.h
 */

#ifndef _ENCODING_GL_CONTEXT_H_
#define _ENCODING_GL_CONTEXT_H_

#include <wrench/gl/Texture.h>

using namespace std;

class EncodingGLContext
{
public:
  virtual ~EncodingGLContext(void) {};

  virtual Texture* decode() = 0;
};

#endif	// _ENCODING_GL_CONTEXT_H_
