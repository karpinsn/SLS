#ifndef _HOLOVIDEO_ENCODER_H_
#define _HOLOVIDEO_ENCODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

#include "Holodecoder.h"
#include "Holoencoder.h"

using namespace std;

class HolovideoEncoder : public Codec
{
private:
  VideoIO m_io;
  Holoencoder m_encoder;
  EncodingOpenGLWidget* m_glWidget;

  string m_filename;

public:
  HolovideoEncoder();
  void initCodec(string& filename, int width, int height);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  void process(MeshInterchange* data);
  void previewProcess(MeshInterchange* data);

  static string codecName(void);
};

#endif  //_HOLOVIDEO_ENCODER_H_
