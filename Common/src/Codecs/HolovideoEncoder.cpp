#include "HolovideoEncoder.h"

HolovideoEncoder::HolovideoEncoder()
{
  m_filename  = "";
  m_videoWriter = new reactor::VideoFileWriter();
  m_video = (reactor::VideoFileWriter*)m_videoWriter;
  m_image = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 3);
  m_yuv444toyuv422.init(PIX_FMT_YUV444P, PIX_FMT_YUV422P); 

  yuv444Frame = avcodec_alloc_frame();

  yuv444Frame->width = 512;
  yuv444Frame->height = 512;

  int numBytes = avpicture_get_size(PIX_FMT_YUV444P, 512, 512);
  yuv444Buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
  avpicture_fill((AVPicture*)yuv444Frame, yuv444Buffer, PIX_FMT_YUV444P, 512, 512);
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
  if(nullptr == glWidget)
  {
	//	No OpenGL encoding widget. Cant open encoding stream
	return;
  }

  m_glWidget = glWidget;
  m_glWidget->setEncodingContext(&m_encoder);
  m_glWidget->setGLContext(&m_encoder);
  m_glWidget->reinit(getWidth(), getHeight());

  m_io.openSaveStream(m_filename, getWidth(), getHeight(), 30);

  if(!m_filename.empty())
  {
	//m_video->openFile(m_filename);
  }

  m_videoWriter = new reactor::ColorSpaceWriterFilter(m_video, PIX_FMT_YUV444P);
}

void HolovideoEncoder::closeCodec(void)
{
  //  Close the stream
  //m_video->closeFile();
	m_io.closeSaveStream();
}

#include "ImageIO.h"

shared_ptr<MeshInterchange> HolovideoEncoder::process(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return nullptr;
  }

  m_encoder.setCurrentMesh(data);

  MeshInterchange* mesh = m_glWidget->encode();
  //mesh->getTexture()->transferFromTexture(m_image);

  //static int frameCount = 0;
  
  //char filename[1000];
  //sprintf(filename, "N:/Data/Frames/%04d.png", frameCount++);
  
  //ImageIO io;
  //io.saveImage(filename, m_image, true);
  //cvSaveImage(filename, m_image);


  //m_yuv444toyuv422.iplImage2AVFrame(m_image, yuv444Frame);

  //  Encode to the stream
    //reactor::MediaFrame frame = reactor::MediaFrame(yuv444Frame, PIX_FMT_YUV444P);
    //m_videoWriter->writeFrame(frame);
  m_io.saveStream(*(mesh->getTexture()));

  return nullptr;
}

shared_ptr<MeshInterchange> HolovideoEncoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return nullptr;
  }

  m_encoder.setCurrentMesh(data);

  //  Dont want to encode, only draw
  m_glWidget->updateScene();

  return nullptr;
}

void HolovideoEncoder::autoFitData(void)
{
  m_encoder.autoFitTransforms();

  if(nullptr != m_glWidget)
  {
	m_glWidget->updateScene();
  }
}

string HolovideoEncoder::codecName(void)
{
  return "Holovideo Encoder";
}
