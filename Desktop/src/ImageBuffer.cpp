#include "ImageBuffer.h"

ImageBuffer::ImageBuffer(int size, int imagesToDrop) : m_bufferSize(size), m_dropCount(imagesToDrop)
{	
  m_freeImages = unique_ptr<QSemaphore>(new QSemaphore(size));
  m_queuedImages = unique_ptr<QSemaphore>(new QSemaphore(0));
}

void ImageBuffer::pushFrame(const IplImage *image)
{
  m_lock.lock();

  if(m_freeImages->tryAcquire())
  {
	//	Yay! We got the resources, continue as usual
	shared_ptr<IplImage> temp = shared_ptr<IplImage>(cvCloneImage(image), [](IplImage* ptr) { cvReleaseImage(&ptr); });
	m_imageQueue.enqueue(temp);
	m_queuedImages->release();
  }
  else
  {
	// Make sure this works. Make sure we can drop this many
	assert(m_queuedImages->available() >= m_dropCount);

	//	Shoot! Our buffer is full. Drop some images so we can add more
	for(int i = 0; i < m_dropCount; ++i)
	{
	  m_queuedImages->acquire();
	  m_imageQueue.dequeue();
	  m_freeImages->release();
	}

	//	Should be able to get room now
	assert(m_freeImages->tryAcquire());

	//	We have room now. Now enqueue
	shared_ptr<IplImage> temp = shared_ptr<IplImage>(cvCloneImage(image), [](IplImage* ptr) { cvReleaseImage(&ptr); });
	m_imageQueue.enqueue(temp);
	m_queuedImages->release();
  }

  m_lock.unlock();
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
  return m_bufferSize;
}

int ImageBuffer::bufferCurrentCount(void)
{
  return m_imageQueue.size();
}