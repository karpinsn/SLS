#ifndef _XYZM_CODEC_H_
#define _XYZM_CODEC_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "XYZFileIO.h"
#include "XYZMesh.h"

using namespace std;

class XYZMCodec : public Codec
{
private:
  XYZFileIO m_io;
  EncodingOpenGLWidget* m_glWidget;
  AbstractMesh* m_mesh;

public:
  void openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height);
  void encode(MeshInterchange& data);
  void closeEncodeStream(void);

  void openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename);
  MeshInterchange* decode();
  void closeDecodeStream(void);
  int getDecodeStreamWidth(void);
  int getDecodeStreamHeight(void);
  float getDecodeStreamProgress(void);

  static string codecName(void);
};

#endif  //_XYZM_CODEC_H_
