#ifndef _MULTI_WAVELENGTH_DECODER_H_
#define _MULTI_WAVELENGTH_DECODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"
#include "Capture\NineFringe\NineFringeCapture.h"

using namespace std;

class MultiWavelengthDecoder : public Codec
{
private:
  VideoIO				  m_io;
  NineFringeCapture  m_coder;
  bool					  m_calculateReference;
  EncodingOpenGLWidget*	  m_glWidget;
  string				  m_filename;

public:
  MultiWavelengthDecoder(string& filename);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  void process(MeshInterchange* data);
  void previewProcess(MeshInterchange* data);

  int getWidth(void);
  int getHeight(void);
  float getStreamLocation(void);

  void setGammaCutoff(float gammaValue);
  void setScalingFactor(float scaling);

  static string codecName(void);

private:
  bool _streamUntilNewFrame(void);
};

#endif  //_MULTI_WAVELENGTH_DECODER_H_
