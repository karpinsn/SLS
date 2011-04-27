#include "ImageBuffer.h"

ImageBuffer::ImageBuffer(int size) : m_bufferSize(size)
{	
  m_freeImages = new QSemaphore(size);
  m_queuedImages = new QSemaphore(0);
}

ImageBuffer::~ImageBuffer()
{
}

void ImageBuffer::pushFrame(const IplImage *image)
{
  m_freeImages->acquire();

  IplImage* temp = cvCloneImage(image);
  m_imageQueue.enqueue(temp);

  m_queuedImages->release();
}

IplImage* ImageBuffer::popFrame(void)
{
  m_queuedImages->acquire();

  IplImage* temp = m_imageQueue.dequeue();

  m_freeImages->release();

  return temp;
}

int ImageBuffer::bufferSize(void)
{
  return m_imageQueue.size();
}
