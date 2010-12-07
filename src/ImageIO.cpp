#include "ImageIO.h"

ImageIO::ImageIO(void)
{
	m_videoWriterInUse = false;
	m_videoReaderInUse = false;
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
	ensureImageSize(imageWidth, imageHeight, 3);
	
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, m_imageHandle->imageData);
	
	//	Flip the image and convert to BGR since that is how OpenCV is looking for it
	//cvFlip(m_imageHandle, 0);
	cvCvtColor(m_imageHandle, m_imageHandle, CV_RGB2BGR);
	
	return cvSaveImage(filename.c_str(), m_imageHandle);
}

bool ImageIO::saveImage(const string &filename, const IplImage* image, bool needChannelReorder)
{
	ensureImageSize(image->width, image->height, image->nChannels);
	
	bool saved = false;
	if(IPL_DEPTH_8U == image->depth && (3 == image->nChannels || 1 == image->nChannels || 4 == image->nChannels))
	{
		if(needChannelReorder)
		{
			if(3 == image->nChannels)
			{
				cvCvtColor(image, m_imageHandle, CV_RGB2BGR);
				saved = cvSaveImage(filename.c_str(), m_imageHandle);
			}
			else if(4 == image->nChannels)
			{
				cvCvtColor(image, m_imageHandle, CV_RGBA2BGRA);
				saved = cvSaveImage(filename.c_str(), m_imageHandle);
			}
		}
		else
		{
			saved = cvSaveImage(filename.c_str(), image);
		}
	}
	
	return saved;
}

bool ImageIO::saveTexture(const string &filename, TextureFacade &texture)
{
	ensureImageSize(texture.getWidth(), texture.getHeight(), texture.getChannelCount());
	
	bool saved = false;
	if(GL_UNSIGNED_BYTE == texture.getDataType())
	{
		texture.transferFromTexture(m_imageHandle);
		
		bool reorderChannels = texture.getFormat() == GL_RGBA || texture.getFormat() == GL_RGB;
		saved = saveImage(filename, m_imageHandle, reorderChannels);
	}
	
	return saved;
}

IplImage* ImageIO::readImage(const string &filename)
{
	IplImage* image = cvLoadImage(filename.c_str());
	
	if(!image)
	{
		clog << "Unable to read image: " << filename << endl;
	}
	else
	{
		cvCvtColor(image, image, CV_BGR2RGB);
	}

	
	return image;
}

bool ImageIO::saveAviFile(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps)
{
	bool openedVideoWriter = false;
	
	if(!m_videoWriterInUse)
	{
		m_videoWriterHandle = cvCreateVideoWriter(filename.c_str(), -1, fps, cvSize(videoWidth, videoHeight), 1);
		m_videoWriterInUse = true;
		
		openedVideoWriter = m_videoWriterInUse;
	}
	else
	{
		clog << "A video stream is already in use. Cannot open another one till the first one is closed" << endl;
	}

	return openedVideoWriter;
}

bool ImageIO::saveAviFileWriteFrame(TextureFacade& texture)
{
	if (m_videoWriterInUse) 
	{
		ensureImageSize(texture.getWidth(), texture.getHeight(), texture.getChannelCount());
		texture.transferFromTexture(m_imageHandle);
		
		if(texture.getFormat() == GL_RGBA || texture.getFormat() == GL_RGB)
		{
			if(3 == m_imageHandle->nChannels)
			{
				cvCvtColor(m_imageHandle, m_imageHandle, CV_RGB2BGR);
			}
			else if(4 == m_imageHandle->nChannels)
			{
				cvCvtColor(m_imageHandle, m_imageHandle, CV_RGBA2BGRA);
			}
		}
	
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

bool ImageIO::readAviFile(const string &filename)
{
	bool openedVideoReader = false;
	
	if(!m_videoReaderInUse)
	{
		m_videoReaderHandle = cvCaptureFromAVI(filename.c_str());
		m_videoReaderInUse = true;
		
		openedVideoReader = m_videoReaderInUse;
	}
	else
	{
		clog << "A video reader is already in use. Cannot open another one till the first one is closed" << endl;
	}
	
	return openedVideoReader;
}

IplImage* ImageIO::readAviFileFrame()
{
	IplImage *frame = NULL;
	
	if(m_videoReaderInUse)
	{
		frame = cvQueryFrame(m_videoReaderHandle);
	}
	else 
	{
		clog << "Unable to read frame as no current video reader handle exists" << endl;
	}

	return frame;
}

bool ImageIO::readAviFileFinish(void)
{
	bool successfullyClosed = false;
	
	if (m_videoReaderInUse)
	{
		cvReleaseCapture(&m_videoReaderHandle);
		m_videoReaderInUse = false;
		successfullyClosed = true;
	}
	else 
	{
		clog << "Attempt to close video reader before it was opened" << endl;
	}
	
	return successfullyClosed;
}

bool ImageIO::aviFileOpen(void)
{
	return m_videoReaderInUse;
}

void ImageIO::ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight, const unsigned int channelCount)
{
	//	Check and see if the image is the correct size. If it is do nothing
	if (m_imageWidth != imageWidth || m_imageHeight != imageHeight || (unsigned int)m_imageHandle->nChannels != channelCount)
	{
		cvReleaseImage(&m_imageHandle);
		m_imageHandle = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, channelCount);
		m_imageWidth = imageWidth;
		m_imageHeight = imageHeight;
	}
}