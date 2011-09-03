#ifndef _CODEC_H_
#define _CODEC_H_

#include "MeshInterchange.h"
#include "EncodingOpenGLWidget.h"

class Codec
{
public:
  virtual ~Codec() {};

  virtual void openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height) = 0;
  virtual void encode(MeshInterchange& data) = 0;
  virtual void closeEncodeStream(void) = 0;

  virtual void openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename) = 0;
  virtual MeshInterchange* decode() = 0;
  virtual void closeDecodeStream(void) = 0;
  virtual int getDecodeStreamWidth(void) = 0;
  virtual int getDecodeStreamHeight(void) = 0;
  virtual float getDecodeStreamProgress(void) = 0;
};

#endif  // _CODEC_H_
