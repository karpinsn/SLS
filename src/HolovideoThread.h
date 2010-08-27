#ifndef _HolovideoThread_H
#define _HolovideoThread_H

#include <QThread>

#include "Holodecoder.h"
#include "ImageIO.h"
#include "OpenGLWidget.h"

using namespace std;

class HolovideoThread : public QThread
{
private:
	ImageIO m_aviIO;
	Holodecoder* m_holoDecoder;
	OpenGLWidget* m_glWidget;
	
public:
	HolovideoThread(Holodecoder* holoDecoder, OpenGLWidget* glWidget);
	void run(void);
};

#endif
