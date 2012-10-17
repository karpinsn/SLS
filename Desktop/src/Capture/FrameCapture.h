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

#include "ImageBuffer.h"

class FrameCapture : public QThread
{
  Q_OBJECT

private:
  bool m_running;
  ImageBuffer* m_buffer;

signals:
  void newFrame(shared_ptr<IplImage>);

public:
    FrameCapture();
    ~FrameCapture();

    void init(ImageBuffer* buffer);

protected:
  void run();
 };

#endif	// _FRAME_CAPTURE_H_
