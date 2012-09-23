#ifndef _DEPTH_ENCODER_H_
#define _DEPTH_ENCODER_H_

#include <cv.h>
#include <highgui.h>

#include <string>
#include "Codec.h"
#include "MeshInterchange.h"

#include "VideoIO.h"

using namespace std;

class DepthEncoder : public Codec
{
private:
  VideoIO m_io;

  IplImage* m_floatImageHandleThreeChannel;
  IplImage* m_floatImageHandleSingleChannel;
  IplImage* m_byteImageHandle;

  string m_filename;

  bool m_stretchContrast;
  float m_minContrastValue;
  float m_maxContrastValue;

public:
  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  void process(MeshInterchange* data);
  void previewProcess(MeshInterchange* data);

  void enableContrastStretching(float min, float max);
  void setFilename(string filename);
  void disableContrastStretching();

  static string codecName(void);
};

#endif  //_DEPTH_ENCODER_H_
