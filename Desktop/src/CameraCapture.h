/*
 Filename:		CameraCapture.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
*/

#ifndef _CAMERA_CAPTURE_H_
#define _CAMERA_CAPTURE_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include <cv.h>
#include <highgui.h>

#include "ImageBuffer.h"

class CameraCapture : public QThread
{
public:
    CameraCapture();
    ~CameraCapture();

    void init(ImageBuffer *buffer);
    bool isConnected();
    int getWidth();
    int getHeight();
    int pause();
    int resume();

protected:
    void run();

private:
    CvCapture *m_capture;
    ImageBuffer *m_buffer;

    QMutex m_pauseMutex;
    QWaitCondition m_pauseCondition;
    bool m_paused;
 };

#endif	// _CAMERA_CAPTURE_H_
