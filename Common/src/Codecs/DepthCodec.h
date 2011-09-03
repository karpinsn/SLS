#ifndef _DEPTH_CODEC_H_
#define _DEPTH_CODEC_H_

#include <cv.h>
#include <highgui.h>

#include <string>
#include "Codec.h"
#include "MeshInterchange.h"

#include "VideoIO.h"

using namespace std;

class DepthCodec : public Codec
{
private:
  VideoIO m_io;

  IplImage* m_floatImageHandle;
  IplImage* m_floatImageHandle2;
  IplImage* m_byteImageHandle;

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

#endif  //_DEPTH_CODEC_H_
