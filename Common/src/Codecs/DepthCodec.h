#ifndef _DEPTH_CODEC_H_
#define _DEPTH_CODEC_H_

#include <string>
#include "Codec.h"
#include "MeshInterchange.h"

#include "VideoIO.h"

using namespace std;

class DepthCodec : public Codec
{
private:
  VideoIO m_io;

public:
  void openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height);
  void encode(MeshInterchange& data);
  void closeEncodeStream(void);

  void openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename);
  MeshInterchange* decode();
  void closeDecodeStream(void);
};

#endif  //_DEPTH_CODEC_H_
