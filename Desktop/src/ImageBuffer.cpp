#include "ImageBuffer.h"

ImageBuffer::ImageBuffer(int size) : m_bufferSize(size)
{	
  m_freeImages = unique_ptr<QSemaphore>(new QSemaphore(size));
  m_queuedImages = unique_ptr<QSemaphore>(new QSemaphore(0));
}

ImageBuffer::~ImageBuffer()
{ }

void ImageBuffer::pushFrame(const IplImage *image)
{
  m_freeImages->acquire();

  shared_ptr<IplImage> temp = shared_ptr<IplImage>(cvCloneImage(image), [](IplImage* ptr) { cvReleaseImage(&ptr); });

  m_lock.lock();
  m_imageQueue.enqueue(temp);
  m_lock.unlock();

  m_queuedImages->release();
}

shared_ptr<IplImage> ImageBuffer::popFrame(void)
{
  m_queuedImages->acquire();

  m_lock.lock();
  shared_ptr<IplImage> temp = m_imageQueue.dequeue();
  m_lock.unlock();

  m_freeImages->release();

  return temp;
}

int ImageBuffer::bufferSize(void)
{
  return m_imageQueue.size();
}
