#include "ImageIO.h"

ImageIO::ImageIO(void)
{
	m_imageWidth = 1;
	m_imageHeight = 1;
	
	m_imageHandle = cvCreateImage(cvSize(m_imageWidth, m_imageHeight), IPL_DEPTH_8U, 3);
}

ImageIO::~ImageIO()
{
    cvReleaseImage(&m_imageHandle);   //  TODO comeback and fix this
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

bool ImageIO::saveTexture(const string &filename, Texture &texture)
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
