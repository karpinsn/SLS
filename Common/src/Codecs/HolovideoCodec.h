#ifndef _HOLOVIDEO_CODEC_H_
#define _HOLOVIDEO_CODEC_H_

#include <string>
#include "../Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

using namespace std;

class HolovideoCodec : public Codec
{
private:
  VideoIO m_io;

public:
  void openEncodeStream(string& filename, int width, int height);
  void encode(MeshInterchange& data);
  void closeEncodeStream(void);
};

#endif  //_HOLOVIDEO_CODEC_H_
