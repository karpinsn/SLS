#ifndef _SIX_FRINGE_DECODER_H_
#define _SIX_FRINGE_DECODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"
#include "Capture\SixFringe\SixFringeCapture.h"

using namespace std;

class SixFringeDecoder : public Codec
{
private:
  VideoIO				  m_io;
  SixFringeCapture		  m_coder;
  bool					  m_calculateReference;
  EncodingOpenGLWidget*	  m_glWidget;
  string				  m_filename;

public:
  SixFringeDecoder(string& filename);

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

#endif  //_SIX_FRINGE_DECODER_H_
