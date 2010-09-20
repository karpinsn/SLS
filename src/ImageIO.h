#ifndef IMAGE_IO_HEADER
#define IMAGE_IO_HEADER

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <windows.h>
	#include <gl.h>
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
	
	CvCapture *m_videoReaderHandle;
	bool m_videoReaderInUse;
	
	IplImage *m_imageHandle;
	unsigned int m_imageHeight;
	unsigned int m_imageWidth;
	
public:
	ImageIO(void);
	~ImageIO();
	
	//	Save Image Methods
	bool saveRGBImage(const string &filename, const unsigned int imageWidth, const unsigned int imageHeight);
	bool saveRGBImage(const string &filename, GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight);

	//	Read Image Methods
	IplImage* readImage(const string &filename);
	
	//	Save Avi File Methods
	bool saveAviFile(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps);
	bool saveAviFileWriteFrame(GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight);
	bool saveAviFileFinish(void);
	
	//	Read Avi File Methods
	bool readAviFile(const string &filename);
	IplImage* readAviFileFrame();
	bool readAviFileFinish(void);
	
private:
	void textureToImageHandle(GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight);
	void ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight);
};

#endif // IMAGE_IO_HEADER