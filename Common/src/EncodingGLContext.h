/**
 @file		EncodingGLContext.h
 */

#ifndef _ENCODING_GL_CONTEXT_H_
#define _ENCODING_GL_CONTEXT_H_

#include "MeshInterchange.h"

using namespace std;

class EncodingGLContext
{
public:
  virtual ~EncodingGLContext(void) {};

  virtual void encode() = 0;
  virtual MeshInterchange* getEncodedData() = 0;
};

#endif	// _ENCODING_GL_CONTEXT_H_
