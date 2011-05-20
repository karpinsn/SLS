/*
 Filename:		JAICamera.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _JAI_CAMERA_H_
#define _JAI_CAMERA_H_

#include "Camera.h"

using namespace std;

namespace lens
{
    class JAICamera : public Camera
	{

    public:
      virtual void init(void);
      virtual void open(void);
      virtual void close(void);
	  virtual std::string cameraName(void);
	};
}

#endif	// _JAI_CAMERA_H_
