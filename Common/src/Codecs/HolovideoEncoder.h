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
#include <reactor/MediaFrameWriter.h>
#include <reactor/ColorSpaceWriterFilter.h>

//	FFmpeg Includes
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
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
  reactor::MediaFrameWriter* m_videoWriter;
  reactor::VideoFileWriter* m_video;

  reactor::FormatConverter m_yuv444toyuv422;

  AVFrame* yuv444Frame;
  AVFrame* yuv422Frame;
  uint8_t* yuv444Buffer;
  uint8_t* yuv422Buffer;


  Holoencoder m_encoder;
  EncodingOpenGLWidget* m_glWidget;

  string m_filename;

  shared_ptr<IplImage> m_image;

public:
  HolovideoEncoder();
  void initCodec(string& filename, int width, int height);

  void openCodec(EncodingOpenGLWidget* glWidget);
  void closeCodec(void);
  shared_ptr<MeshInterchange> process(shared_ptr<MeshInterchange> data);
  shared_ptr<MeshInterchange> previewProcess(shared_ptr<MeshInterchange> data);
  void autoFitData(void);
  static string codecName(void);

  // Overrides. Since we can have width and height different than
  // the decoder override so our settings are not overrides
  virtual void setWidth(int width)
  { }

  virtual void setHeight(int height)
  { }
};

#endif  //_HOLOVIDEO_ENCODER_H_
