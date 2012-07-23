/**
 @file		IEncodingGLContext.h
 */

#ifndef _I_ENCODING_GL_CONTEXT_H_
#define _I_ENCODING_GL_CONTEXT_H_

#include "MeshInterchange.h"

using namespace std;

class IEncodingGLContext
{
public:
  virtual ~IEncodingGLContext(void) { };

  virtual void encode() = 0;
  virtual MeshInterchange* getEncodedData() = 0;
};

#endif	// _I_ENCODING_GL_CONTEXT_H_
