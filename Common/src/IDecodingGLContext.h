/**
 @file		IDecodingGLContext.h
 */

#ifndef _I_DECODING_GL_CONTEXT_H_
#define _I_DECODING_GL_CONTEXT_H_

#include "MeshInterchange.h"

using namespace std;

class IDecodingGLContext
{
public:
  virtual ~IDecodingGLContext(void) { };
  virtual MeshInterchange* decode() = 0;
};

#endif	// _I_DECODING_GL_CONTEXT_H_
