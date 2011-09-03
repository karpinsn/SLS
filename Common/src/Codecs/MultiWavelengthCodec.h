#ifndef _MULTI_WAVELENGTH_CODEC_H_
#define _MULTI_WAVELENGTH_CODEC_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"
#include "MultiWavelengthCapture.h"

using namespace std;

class MultiWavelengthCodec : public Codec
{
private:
  VideoIO m_io;
  MultiWavelengthCapture m_coder;
  bool m_calculateReference;
  EncodingOpenGLWidget* m_glWidget;

public:
  void openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height);
  void encode(MeshInterchange& data);
  void closeEncodeStream(void);

  void openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename);
  MeshInterchange* decode();
  void closeDecodeStream(void);
  int getDecodeStreamWidth(void);
  int getDecodeStreamHeight(void);
  float getDecodeStreamProgress(void);

  static string codecName(void);

private:
  bool _streamUntilNewFrame(void);
};

#endif  //_MULTI_WAVELENGTH_CODEC_H_
