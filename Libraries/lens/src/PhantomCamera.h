/**
 @filename		PhantomCamera.h
 @author		Nikolaus Karpinsky

 Provides a unified Lens interface to phantom cameras
 */

#ifndef _PHANTOM_CAMERA_H_
#define _PHANTOM_CAMERA_H_
#define USE_PHANTOM_CAMERA

#include <Phcon.h>

#include "Camera.h"

using namespace std;

namespace lens
{
    class PhantomCamera : public Camera, QThread
	{
	private:
		IplImage*		m_cameraImage;
        bool            m_running;

        unsigned int    m_cameraNumber;
    public:
      PhantomCamera(void);
      virtual void init(void);
      virtual void open(void);
      virtual void close(void);
	  virtual float getWidth(void);
      virtual float getHeight(void);

      static std::string cameraName(void);

	private:
		void _openFactory(void);
		void _openCamera(void);

		void _closeFactory(void);
		void _closeCamera(void);
	};
}

#endif	// _JAI_CAMERA_H_
