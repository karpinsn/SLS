#ifndef IMAGE_IO_HEADER
#define IMAGE_IO_HEADER

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
#endif

#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>

using namespace std;

class ImageIO
{
private:
	CvVideoWriter *m_videoWriterHandle;
	bool m_videoWriterInUse;
	
	IplImage *m_imageHandle;
	unsigned int m_imageHeight;
	unsigned int m_imageWidth;
	
public:
	ImageIO(void);
	~ImageIO();
	
	bool saveRGBImage(const string &filename, const unsigned int imageWidth, const unsigned int imageHeight);
	bool saveRGBImage(const string &filename, GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight);
	bool saveAviFile(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps);
	bool saveAviFileWriteFrame(GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight);
	bool saveAviFileFinish(void);
	
private:
	void textureToImageHandle(GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight);
	void ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight);
};

#endif // IMAGE_IO_HEADER