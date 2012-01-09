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

  IplImage* m_floatImageHandleThreeChannel;
  IplImage* m_floatImageHandleSingleChannel;
  IplImage* m_byteImageHandle;

  bool m_stretchContrast;
  float m_minContrastValue;
  float m_maxContrastValue;

public:
  void openEncodeStream(EncodingOpenGLWidget* glWidget);
  void encode(MeshInterchange& data);
  void previewEncode(MeshInterchange& data);
  void closeEncodeStream(void);

  void openDecodeStream(EncodingOpenGLWidget* glWidget);
  MeshInterchange* decode();
  void closeDecodeStream(void);
  int getDecodeStreamWidth(void);
  int getDecodeStreamHeight(void);
  float getDecodeStreamProgress(void);

  void enableContrastStretching(float min, float max);
  void disableContrastStretching();

  static string codecName(void);
};

#endif  //_DEPTH_CODEC_H_
