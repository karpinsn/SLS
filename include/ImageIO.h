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
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <memory>
#include <iostream>
#include <string>

#include <cv.h>
#include <highgui.h>

#include <wrench/gl/Texture.h>

using namespace std;
using namespace wrench::gl;

class ImageIO
{
private:
	shared_ptr<IplImage> m_imageHandle;
	unsigned int m_imageHeight;
	unsigned int m_imageWidth;
	
public:
	ImageIO(void);
	
	//	Save Image Methods
	bool saveImage(const string &filename, const IplImage* image, bool needChannelReorder = false);
	bool saveTexture(const string &filename, Texture& texture);
	bool saveTexture(const string &filename, Texture* texture);
	bool saveRGBImage(const string &filename, const unsigned int imageWidth, const unsigned int imageHeight);
	
	//	Read Image Methods
	IplImage* readImage(const string &filename);
	
private:
	void ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight, const unsigned int channelCount);
};

#endif // _IMAGE_IO_H_
