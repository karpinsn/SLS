/*
 Filename:		ImageIO.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _IMAGE_IO_H_
#define _IMAGE_IO_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <OpenGL/gl.h>
#elif _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <iostream>
#include <string>

#include <cv.h>
#include <highgui.h>

#include "TextureFacade.h"

using namespace std;

class ImageIO
{
private:
	CvVideoWriter *m_videoWriterHandle;
	bool m_videoWriterInUse;
	
	CvCapture *m_videoReaderHandle;
	bool m_videoReaderInUse;
	
	IplImage *m_imageHandle;
	unsigned int m_imageHeight;
	unsigned int m_imageWidth;
	
public:
	ImageIO(void);
	~ImageIO();
	
	//	Save Image Methods
	bool saveImage(const string &filename, const IplImage* image, bool needChannelReorder = false);
	bool saveTexture(const string &filename, TextureFacade& texture);
	bool saveRGBImage(const string &filename, const unsigned int imageWidth, const unsigned int imageHeight);
	
	//	Read Image Methods
	IplImage* readImage(const string &filename);
	
	//	Save Avi File Methods
	bool saveAviFile(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps);
	bool saveAviFileWriteFrame(TextureFacade& texture);
	bool saveAviFileFinish(void);
	
	//	Read Avi File Methods
	bool readAviFile(const string &filename);
	IplImage* readAviFileFrame();
	bool readAviFileFinish(void);
	bool aviFileOpen(void);
	
private:
	void ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight, const unsigned int channelCount);
};

#endif // _IMAGE_IO_H_
