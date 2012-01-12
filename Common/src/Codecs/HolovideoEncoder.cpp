#include "HolovideoEncoder.h"

HolovideoEncoder::HolovideoEncoder()
{
  m_filename  = "";
  m_image = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 3);
  m_yuv444toyuv422.init(PIX_FMT_YUV444P, PIX_FMT_YUV422P); 

  yuv444Frame = avcodec_alloc_frame();
  yuv422Frame = avcodec_alloc_frame();

  yuv444Frame->width = 512;
  yuv444Frame->height = 512;
  yuv422Frame->width = 512;
  yuv422Frame->height = 512;

  int numBytes = avpicture_get_size(PIX_FMT_YUV444P, 512, 512);
  yuv444Buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
  avpicture_fill((AVPicture*)yuv444Frame, yuv444Buffer, PIX_FMT_YUV444P, 512, 512);

  numBytes = avpicture_get_size(PIX_FMT_YUV422P, 512, 512);
  yuv422Buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
  avpicture_fill((AVPicture*)yuv422Frame, yuv422Buffer, PIX_FMT_YUV422P, 512, 512);
}

HolovideoEncoder::~HolovideoEncoder()
{
  cvReleaseImage(&m_image);
}

void HolovideoEncoder::initCodec(string& filename, int width, int height)
{
  setWidth(width);
  setHeight(height);
  m_filename  = filename;
}

void HolovideoEncoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  //  Open the stream to write to
  if(NULL == glWidget)
  {
	//	No OpenGL encoding widget. Cant open encoding stream
	return;
  }

  m_glWidget = glWidget;
  m_glWidget->setEncodingContext(&m_encoder);
  m_glWidget->setGLContext(&m_encoder);
  m_glWidget->reinit(getWidth(), getHeight());

  if(!m_filename.empty())
  {
	m_videoWriter.openFile(m_filename);
  }
  //m_io.openSaveStream(m_filename, getWidth(), getHeight(), 30);
}

void HolovideoEncoder::closeCodec(void)
{
  //  Close the stream
  m_videoWriter.closeFile();
  //m_io.closeSaveStream();
}

void HolovideoEncoder::process(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(data);

  MeshInterchange* mesh = m_glWidget->encode();
  mesh->getTexture()->transferFromTexture(m_image);

  m_yuv444toyuv422.iplImage2AVFrame(m_image, yuv444Frame);
  m_yuv444toyuv422.convert(reactor::VideoFrame(yuv444Frame, PIX_FMT_YUV444P), reactor::VideoFrame(yuv422Frame, PIX_FMT_YUV422P));

  //  Encode to the stream
  m_videoWriter.writeFrame(reactor::VideoFrame(yuv422Frame, PIX_FMT_YUV422P));
  //m_io.saveStream(*(mesh->getTexture()));
}

void HolovideoEncoder::previewProcess(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(data);

  //  Dont want to encode, only draw
  m_glWidget->updateScene();
}

string HolovideoEncoder::codecName(void)
{
  return "Holovideo Encoder";
}