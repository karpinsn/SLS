/*
 Filename:		FrameCapture.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
*/

#ifndef _FRAME_CAPTURE_H_
#define _FRAME_CAPTURE_H_

#include <QThread>
#include <QMetaType>

#include <cv.h>
#include <highgui.h>

#include "OpenGLWidget.h"
#include "ImageBuffer.h"

/**
* Worker class that runs on the thread. It pulls from the image buffer and
* then calls the callback
*/
class FrameCaptureWorker : public QObject
{
  Q_OBJECT

  private:
	bool m_running;
	ImageBuffer* m_buffer;
	void* m_callbackInstance;
	void (*m_newFrameCallback)(void* callbackInstance, shared_ptr<IplImage> newFrame);
	unique_ptr<QGLWidget> m_oglContext;

  public:
	FrameCaptureWorker(void* callbackInstance, void (*newFrameCallback)(void* callbackInstance, shared_ptr<IplImage> newFrame));
	void init(ImageBuffer* buffer, QGLWidget* context);
	void stop(void);

  signals:
	void finished(void);

  public slots:
	void grabFrames();
};

class FrameCapture : public QObject
{
  Q_OBJECT

private:
  QThread* m_workerThread;
  FrameCaptureWorker* m_worker;

public:
	FrameCapture(void* callbackInstance, void (*newFrameCallback)(void* callbackInstance, shared_ptr<IplImage> newFrame));
	void init(ImageBuffer* buffer, QGLWidget* context);
	void start(void);
 };


#endif	// _FRAME_CAPTURE_H_
