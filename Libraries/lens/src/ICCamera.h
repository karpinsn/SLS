/*
 Filename:		ICCamera.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _IC_CAMERA_H_
#define _IC_CAMERA_H_

#define USE_IC_CAMERA
#include "Camera.h"

using namespace std;

namespace lens
{
    class ICCamera : public Camera
	{
    private:
      //DShowLib::Grabber        m_grabber; // The instance of the Grabber class.

    public:
      virtual void init(void);
      virtual void open(void);
      virtual void close(void);
      static std::string cameraName(void);

	};
}

#endif	// _IC_CAMERA_H_
