/*
 Filename:		ImageBuffer.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11

 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
*/

#ifndef _IMAGE_BUFFER_H_
#define _IMAGE_BUFFER_H_

#include <memory>

#include <qthread.h>

#include <QWaitCondition>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>

#include <cv.h>

using namespace std;

class ImageBuffer
{
public:
  ImageBuffer(int size = 60);

  void pushFrame(const IplImage *image);
  shared_ptr<IplImage> popFrame(void);

  /**
  *	Returns the maximum size of this ImageBuffer
  *
  *	@return Maximum size of this ImageBuffer
  */
  int bufferSize(void);

  /**
  * Returns the current number of images in this ImageBuffer
  *
  *	@return Number of images in this ImageBuffer
  */
  int bufferCurrentCount(void);

private:
  const int m_bufferSize;
  unique_ptr<QSemaphore> m_freeImages;
  unique_ptr<QSemaphore> m_queuedImages;
  QMutex     m_lock;
  QQueue<shared_ptr<IplImage> > m_imageQueue;

};

#endif	// _IMAGE_BUFFER_H_
