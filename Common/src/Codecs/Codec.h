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
  virtual shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data) = 0;
  virtual shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data) = 0;

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
