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

#include <QWaitCondition>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>

#include <cv.h>

class ImageBuffer
{
public:
  ImageBuffer(int size = 2);
  ~ImageBuffer();

  void pushFrame(const IplImage *image);
  IplImage* popFrame(void);
  int bufferSize(void);

private:
  const int m_bufferSize;
  QSemaphore *m_freeImages;
  QSemaphore *m_queuedImages;
  QQueue<IplImage*> m_imageQueue;

};

#endif	// _IMAGE_BUFFER_H_
