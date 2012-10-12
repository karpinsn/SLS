#ifndef _NINE_FRINGE_DECODER_H_
#define _NINE_FRINGE_DECODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"
#include "Capture\NineFringe\NineFringeCapture.h"

using namespace std;

class NineFringeDecoder : public Codec
{
private:
  VideoIO				  m_io;
  NineFringeCapture  m_coder;
  bool					  m_calculateReference;
  EncodingOpenGLWidget*	  m_glWidget;
  string				  m_filename;

public:
  NineFringeDecoder(string& filename);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data);
  shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data);

  int getWidth(void);
  int getHeight(void);
  float getStreamLocation(void);

  void setGammaCutoff(float gammaValue);
  void setScalingFactor(float scaling);

  static string codecName(void);

private:
  bool _streamUntilNewFrame(void);
};

#endif  //_NINE_FRINGE_DECODER_H_
