#include "ViewController.h"

ViewController::ViewController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
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

void ViewController::playMovie(string movieFile)
{
  if(!m_aviIO.aviFileOpen())
  {
    bool fileOpened = m_aviIO.readAviFile(movieFile.c_str());

    if(fileOpened)
    {
      IplImage *frame = m_aviIO.readAviFileFrame();
      if(frame)
      {
        m_decoder.setBackHoloBuffer(frame);
        m_decoder.swapBuffers();
        m_movieTimer.start();
        startTimer(0);
      }
    }
  }

  _updateGL();
}

void ViewController::timerEvent(QTimerEvent* event)
{
  int elapsed = m_movieTimer.elapsed();

  if(elapsed >= 33)
  {
    m_movieTimer.restart();
    //	Need to fetch the next frame

    IplImage* frame = m_aviIO.readAviFileFrame();

    if(frame)
    {
      m_decoder.setBackHoloBuffer(frame);
      m_decoder.swapBuffers();
    }
    else
    {
      //killTimer(event->id);
    }

  }
  _updateGL();
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
