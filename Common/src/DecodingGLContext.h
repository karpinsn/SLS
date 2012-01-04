/**
 @file		DecodingGLContext.h
 */

#ifndef _DECODING_GL_CONTEXT_H_
#define _DECODING_GL_CONTEXT_H_

#include "MeshInterchange.h"

using namespace std;

class DecodingGLContext
{
public:
  virtual ~DecodingGLContext(void) {};
  virtual MeshInterchange* decode() = 0;
};

#endif	// _DECODING_GL_CONTEXT_H_
