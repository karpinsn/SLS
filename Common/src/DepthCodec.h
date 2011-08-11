#ifndef _DEPTH_CODEC_H_
#define _DEPTH_CODEC_H_

#include "Codec.h"
#include "MeshInterchange.h"

#include "VideoIO.h"

class DepthCodec : public Codec
{
private:
  VideoIO m_io;

public:
  void openEncodeStream(int width, int height);
  void encode(MeshInterchange& data);
  void closeEncodeStream(void);
};

#endif  //_DEPTH_CODEC_H_
