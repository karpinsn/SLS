/**
 @filename		PhantomCamera.h
 @author		Nikolaus Karpinsky

 Provides a unified Lens interface to phantom cameras
 */

#ifndef _PHANTOM_CAMERA_H_
#define _PHANTOM_CAMERA_H_
#define USE_PHANTOM_CAMERA

#ifdef _WIN32
	#include <windows.h>	//	Needed by windows for typedefs in phcon.h
#endif

#include <iostream>
#include <phcon.h>			//	Phantom camera includes
#include <QThread>			//	Qt Includes

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

		//	Bitmap Info structure used by the camera
		BYTE			CineBMI[sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)];
		LPBITMAPINFO    m_CineBMI;

		//	Acquisition parameters of the camera
		PACQUIPARAMS	m_params;

    public:
      PhantomCamera(void);
	  ~PhantomCamera();

      virtual void init(void);
      virtual void open(void);
      virtual void close(void);
	  virtual float getWidth(void);
      virtual float getHeight(void);

	  void setFrameRate(int fps);
	  void setTrigger(unsigned int triggerMode);

      static std::string cameraName(void);

    protected:
      void run();

	private:
		void _openFactory(void);
		void _openCamera(void);

		void _closeFactory(void);
		void _closeCamera(void);
	};
}

#endif	// _JAI_CAMERA_H_
