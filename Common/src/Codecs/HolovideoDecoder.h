#ifndef _HOLOVIDEO_DECODER_H_
#define _HOLOVIDEO_DECODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

#include "Holodecoder.h"
#include "Encode/Holoencoder.h"

using namespace std;

class HolovideoDecoder: public Codec
{
private:
  VideoIO m_io;
  Holodecoder m_decoder;
  EncodingOpenGLWidget* m_glWidget;

  string m_filename;

public:
  HolovideoDecoder();
  void initCodec(string& filename);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data);
  shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data);

  int getWidth(void);
  int getHeight(void);
  float getStreamLocation(void);

  static string codecName(void);
};

#endif  //_HOLOVIDEO_DECODER_H_
