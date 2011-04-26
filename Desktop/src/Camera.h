/*
 Filename:		CameraView.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <QThread>

#include "opencv/highgui.h"

class Camera : public QThread
{
public:
    Camera(int deviceNumber);
    ~Camera();

    bool isConnected();
protected:
    void run();

private:
    CvCapture *m_capture;
 };

#endif	// _CAMERA_H_
