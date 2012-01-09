#ifndef _CODEC_H_
#define _CODEC_H_

#include "MeshInterchange.h"
#include "EncodingOpenGLWidget.h"

class Codec
{
protected:
  int m_width;
  int m_height;

public:
  virtual ~Codec() {};

  virtual void openCodec(EncodingOpenGLWidget* glWidget) = 0;
  virtual void closeCodec(void) = 0;
  virtual void process(MeshInterchange* data) = 0;
  virtual void previewProcess(MeshInterchange* data) = 0;

  //virtual void openEncodeStream(EncodingOpenGLWidget* glWidget) = 0;
  //virtual void encode(MeshInterchange& data) = 0;
  //virtual void previewEncode(MeshInterchange& data) = 0;
  //virtual void closeEncodeStream(void) = 0;

  //virtual void openDecodeStream(EncodingOpenGLWidget* glWidget) = 0;
  //virtual MeshInterchange* decode() = 0;
  //virtual void closeDecodeStream(void) = 0;
  //virtual float getDecodeStreamProgress(void) = 0;

  virtual int getWidth(void)
  {
	return m_width;
  }

  virtual int getHeight(void)
  {
	return m_height;
  }

  virtual void setWidth(int width)
  {
	m_width = width;
  }

  virtual void setHeight(int height)
  {
	m_height = height;
  }

  virtual float getStreamLocation()
  {
      return 0.0f;
  }
};

#endif  // _CODEC_H_
