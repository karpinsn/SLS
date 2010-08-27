#include "HolovideoThread.h"

HolovideoThread::HolovideoThread(Holodecoder* holoDecoder, OpenGLWidget* glWidget) : QThread()
{
	m_holoDecoder = holoDecoder;
	m_glWidget = glWidget;
}

void HolovideoThread::run(void)
{	
	bool fileOpened = m_aviIO.readAviFile("C:/Users/Nikolaus Karpinsky/Data/HoloVideo.avi");
	
	if(fileOpened)
	{
		
		IplImage *frame = m_aviIO.readAviFileFrame();
		if(frame)
		{
			m_glWidget->makeCurrent();
			m_holoDecoder->setBackHoloBuffer(frame);
			m_holoDecoder->swapBuffers();
			m_glWidget->updateScene();
		}
		
		frame = m_aviIO.readAviFileFrame();
		while (frame) 
		{
			m_holoDecoder->setBackHoloBuffer(frame);
			
			//	Sleep for the desired frame rate
			msleep(33.3);
			
			m_holoDecoder->swapBuffers();
			m_glWidget->updateScene();
			
			frame = m_aviIO.readAviFileFrame();
		}
	}
}