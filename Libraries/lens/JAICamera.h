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
#include <Jai_Factory.h>
//#include <stdint.h>

using namespace std;

namespace lens
{
    class JAICamera : public Camera
	{
	private:
		FACTORY_HANDLE	m_factory; // Factory Handle
		CAM_HANDLE		m_camera; // Camera Handle
		int8_t          m_cameraID[J_CAMERA_ID_SIZE];    // Camera ID string

    public:
      virtual void init(void);
      virtual void open(void);
      virtual void close(void);
	  virtual std::string cameraName(void);

	private:
		void _openFactory(void);
		void _openCamera(void);

		void _closeFactory(void);
		void _closeCamera(void);
	};
}

#endif	// _JAI_CAMERA_H_
