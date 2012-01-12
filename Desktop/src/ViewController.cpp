#include "ViewController.h"

ViewController::ViewController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
  _connectSignalsWithController();
}

ViewController::~ViewController()
{
}

void ViewController::init(void)
{
  viewGLWidget->setGLContext(&m_decoder);
  viewGLWidget->m_holoDecoder = &m_decoder;
}

void ViewController::cameraSelectMode(int mode)
{
  m_decoder.cameraSelectMode(mode);
}

void ViewController::openHoloImage(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select Holoimage to Open", "/", "Images (*.png *.jpg)");

  if(!file.isEmpty())
  {
    ImageIO io;
    IplImage* image = io.readImage(file.toStdString().c_str());

    if(NULL != image)
    {

      m_decoder.setBackHoloBuffer(image);
      m_decoder.swapBuffers();

      _updateGL();
    }
  }
}

#include <reactor/FormatConverter.h>
#include <cv.h>
#include <highgui.h>

#define __STDC_CONSTANT_MACROS
extern "C"
{
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
  #include <libswscale/swscale.h>
}

AVFrame* yuv444Frame;
uint8_t* yuv444Buffer;

IplImage* tempImage;
reactor::FormatConverter m_converter;

void ViewController::playMovie(string movieFile)
{
  if(!m_aviIO.readStreamIsOpen())
  {
	bool fileOpened = m_reader.openFile(movieFile);
    //bool fileOpened = m_aviIO.openReadStream(movieFile);

	//	TODO comeback and fix this
	tempImage = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 3);
	yuv444Frame = avcodec_alloc_frame();
	yuv444Frame->width = 512;
	yuv444Frame->height = 512;

	int numBytes = avpicture_get_size(PIX_FMT_YUV444P, 512, 512);
	yuv444Buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
	avpicture_fill((AVPicture*)yuv444Frame, yuv444Buffer, PIX_FMT_YUV444P, 512, 512);
	m_converter.init(PIX_FMT_YUV422P, PIX_FMT_YUV444P); 

    if(fileOpened)
    {
	  m_converter.convert(m_reader.readFrame(), reactor::VideoFrame(yuv444Frame, PIX_FMT_YUV444P));
	  m_converter.avFrame2IplImage(yuv444Frame, tempImage);

      //IplImage *frame = m_aviIO.readStream();
      if(tempImage)
      {
        m_decoder.setBackHoloBuffer(tempImage);
        m_decoder.swapBuffers();
        m_movieTimer.start();
        startTimer(0);
      }
    }
  }

  _updateGL();
}

void ViewController::pauseMovie()
{
}

void ViewController::timerEvent(QTimerEvent* event)
{
  int elapsed = m_movieTimer.elapsed();

  if(elapsed >= 33)
  {
    m_movieTimer.restart();
    //	Need to fetch the next frame

    //IplImage* frame = m_aviIO.readStream();
	
	float position = m_aviIO.readStreamPosition() * 100.0;
	positionSlider->setValue(position);

    if(frame)
    {
      m_decoder.setBackHoloBuffer(tempImage);
      m_decoder.swapBuffers();
    }
    else
    {
      //killTimer(event->id);
    }

  }
  _updateGL();
}

void ViewController::changeMoviePosition(int position)
{
  m_aviIO.setReadStreamPosition((float)position / 100.0f);
}

void ViewController::_updateGL(void)
{
  OpenGLWidget* glContext = findChild<OpenGLWidget*>(QString::fromUtf8("viewGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("ViewController - _updateGL: Unable to find OpenGL Widget");
  }
}

void ViewController::_connectSignalsWithController(void)
{
  connect(positionSlider, SIGNAL(sliderMoved(int)),	this, SLOT(changeMoviePosition(int)));
  connect(positionSlider, SIGNAL(sliderPressed()),	this, SLOT(pauseMovie()));
}