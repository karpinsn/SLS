#ifndef _HOLOVIDEO_CODEC_H_
#define _HOLOVIDEO_CODEC_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

#include "Holodecoder.h"
#include "Holoencoder.h"

using namespace std;

class HolovideoCodec : public Codec
{
private:
  VideoIO m_io;
  Holodecoder m_decoder;
  Holoencoder m_encoder;
  EncodingOpenGLWidget* m_glWidget;

  int m_width;
  int m_height;
  string m_filename;

public:
  HolovideoCodec();
  void initCodec(string& filename, int width, int height);
  void openEncodeStream(EncodingOpenGLWidget* glWidget);
  void encode(MeshInterchange& data);
  void previewEncode(MeshInterchange& data);
  void closeEncodeStream(void);

  void openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename);
  MeshInterchange* decode();
  void closeDecodeStream(void);
  int getDecodeStreamWidth(void);
  int getDecodeStreamHeight(void);
  float getDecodeStreamProgress(void);

  static string codecName(void);
};

#endif  //_HOLOVIDEO_CODEC_H_
