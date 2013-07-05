/**
 @file
 */

#ifndef _I_ENCODER_H_
#define _I_ENCODER_H_

#include "MeshInterchange.h"

using namespace std;

class IEncoder
{
public:
  virtual ~IEncoder(void) { };
  virtual void encode() = 0;
  virtual MeshInterchange* getEncodedData() = 0;
};

#endif	// _I_ENCODING_GL_CONTEXT_H_
