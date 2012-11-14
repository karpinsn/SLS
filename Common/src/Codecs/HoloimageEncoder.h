#ifndef _HOLOIMAGE_ENCODER_H_
#define _HOLOIMAGE_ENCODER_H_

#include <string>
#include <cstdio>

#include "Codec.h"
#include "../MeshInterchange.h"
#include "../ImageIO.h"

#include "Holodecoder.h"
#include "Encode/SpatialHoloencoder.h"

using namespace std;

class HoloimageEncoder : public Codec
{
private:
  SpatialHoloencoder m_encoder;
  EncodingOpenGLWidget* m_glWidget;

  ImageIO m_io;

  string m_filename;

  static const int m_filenameBufferSize = 5000;
  char m_filenameBuffer[m_filenameBufferSize];

  int m_frameNumber;

public:
  HoloimageEncoder();
  void initCodec(string& filename, int width, int height);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data);
  shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data);
  void autoFitData(void);
  static string codecName(void);

  //	Overrides. Since we can have width and height different than
  //	the decoder override so our settings are not overrides
  virtual void setWidth(int width)
  { }

  virtual void setHeight(int height)
  { }
};

#endif  //_HOLOIMAGE_ENCODER_H_
