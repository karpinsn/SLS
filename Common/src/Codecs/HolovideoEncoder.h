#ifndef _HOLOVIDEO_ENCODER_H_
#define _HOLOVIDEO_ENCODER_H_

#include <string>
#include "Codec.h"
#include "../MeshInterchange.h"

#include "../VideoIO.h"

#include "Holodecoder.h"
#include "Holoencoder.h"

#include <reactor/VideoFileWriter.h>
#include <Reactor/FormatConverter.h>
#include <Reactor/MediaFrame.h>

//	FFmpeg Includes
#define __STDC_CONSTANT_MACROS
extern "C"
{
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
}

using namespace std;

class HolovideoEncoder : public Codec
{
private:
  VideoIO m_io;
  reactor::VideoFileWriter m_videoWriter;
  reactor::FormatConverter m_yuv444toyuv422;

  AVFrame* yuv444Frame;
  AVFrame* yuv422Frame;
  uint8_t* yuv444Buffer;
  uint8_t* yuv422Buffer;


  Holoencoder m_encoder;
  EncodingOpenGLWidget* m_glWidget;

  string m_filename;

  IplImage* m_image;

public:
  HolovideoEncoder();
  ~HolovideoEncoder();
  void initCodec(string& filename, int width, int height);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  void process(MeshInterchange* data);
  void previewProcess(MeshInterchange* data);

  static string codecName(void);
};

#endif  //_HOLOVIDEO_ENCODER_H_
