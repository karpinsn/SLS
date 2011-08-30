#ifndef _MULTI_WAVELENGTH_CODEC_H_
#define _MULTI_WAVELENGTH_CODEC_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

using namespace std;

class MultiWavelengthCodec : public Codec
{
private:
  VideoIO m_io;

public:
  void openEncodeStream(string& filename, int width, int height);
  void encode(MeshInterchange& data);
  void closeEncodeStream(void);
};

#endif  //_MULTI_WAVELENGTH_CODEC_H_
