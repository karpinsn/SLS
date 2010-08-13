#include "ImageIO.h"

ImageIO::ImageIO(void)
{
	m_videoWriterInUse = false;
	m_imageWidth = 1;
	m_imageHeight = 1;
	
	m_imageHandle = cvCreateImage(cvSize(m_imageWidth, m_imageHeight), IPL_DEPTH_8U, 3);
}

ImageIO::~ImageIO()
{
	if (m_videoWriterInUse) 
	{
		clog << "Video writer stream not properly closed. Closing video writer stream" << endl;
		cvReleaseVideoWriter(&m_videoWriterHandle);
	}
	cvReleaseImage(&m_imageHandle);
}

bool ImageIO::saveRGBImage(const string &filename, const unsigned int imageWidth, const unsigned int imageHeight)
{
	ensureImageSize(imageWidth, imageHeight);
	
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, m_imageHandle->imageData);
	
	//	Flip the image and convert to BGR since that is how OpenCV is looking for it
	cvFlip(m_imageHandle, 0);
	cvCvtColor(m_imageHandle, m_imageHandle, CV_RGB2BGR);
	
	return cvSaveImage(filename.c_str(), m_imageHandle);
}

bool ImageIO::saveRGBImage(const string &filename, GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight)
{	
	textureToImageHandle(textureID, imageWidth, imageHeight);
	
	return cvSaveImage(filename.c_str(), m_imageHandle);
}

bool ImageIO::saveAviFile(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps)
{
	bool openedVideoWriter = false;
	
	if(!m_videoWriterInUse)
	{
		m_videoWriterHandle = cvCreateVideoWriter(filename.c_str(), CV_FOURCC('P', 'I', 'M', '1'), fps, cvSize(videoWidth, videoHeight), 1);
		m_videoWriterInUse = true;
		
		openedVideoWriter = m_videoWriterInUse;
	}
	else
	{
		clog << "A video stream is already in use. Cannot open another one till the first one is closed" << endl;
	}

	return openedVideoWriter;
}

bool ImageIO::saveAviFileWriteFrame(GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight)
{	
	if (m_videoWriterInUse) 
	{
		textureToImageHandle(textureID, imageWidth, imageHeight);
		cvWriteFrame(m_videoWriterHandle, m_imageHandle);
	}
	else 
	{
		clog << "Unable to write frame out to file as no current video writer handle exists" << endl;
	}

	return m_videoWriterInUse;
}

bool ImageIO::saveAviFileFinish(void)
{
	bool successfullyClosed = false;
	
	if (m_videoWriterInUse)
	{
		cvReleaseVideoWriter(&m_videoWriterHandle);
		m_videoWriterInUse = false;
		successfullyClosed = true;
	}
	else 
	{
		clog << "Attempt to close video writer before it was opened" << endl;
	}
	
	return successfullyClosed;
}

void ImageIO::textureToImageHandle(GLuint textureID, const unsigned int imageWidth, const unsigned int imageHeight)
{
	ensureImageSize(imageWidth, imageHeight);
	
	//	Bind the texture and stream into the image handle
	glBindTexture(GL_TEXTURE_2D, textureID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, m_imageHandle->imageData);
	
	//	Flip the image and convert to BGR since that is how OpenCV is looking for it
	cvFlip(m_imageHandle, 0);
	cvCvtColor(m_imageHandle, m_imageHandle, CV_RGB2BGR);
}

void ImageIO::ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight)
{
	//	Check and see if the image is the correct size. If it is do nothing
	if (m_imageWidth != imageWidth && m_imageHeight != imageHeight)
	{
		cvReleaseImage(&m_imageHandle);
		m_imageHandle = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 3);
		m_imageWidth = imageWidth;
		m_imageHeight = imageHeight;
	}
}