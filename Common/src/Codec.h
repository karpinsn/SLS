#ifndef _CODEC_H_
#define _CODEC_H_

#include "MeshInterchange.h"

class Codec
{
public:
  virtual ~Codec() {};

  virtual void openEncodeStream(int width, int height) = 0;
  virtual void encode(MeshInterchange& data) = 0;
  virtual void closeEncodeStream(void) = 0;
};

#endif  // _CODEC_H_
