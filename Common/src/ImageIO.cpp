#include "ImageIO.h"

ImageIO::ImageIO(void)
{
	m_imageWidth = 1;
	m_imageHeight = 1;
	
	m_imageHandle = shared_ptr<IplImage>(
	  cvCreateImage(cvSize(m_imageWidth, m_imageHeight), IPL_DEPTH_8U, 3),
	  [](IplImage* ptr){ cvReleaseImage(&ptr); });
}

bool ImageIO::saveRGBImage(const string &filename, const unsigned int imageWidth, const unsigned int imageHeight)
{
	ensureImageSize(imageWidth, imageHeight, 3);
	
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, m_imageHandle->imageData);
	
	//	Flip the image and convert to BGR since that is how OpenCV is looking for it
	cvFlip(m_imageHandle.get(), 0);
	cvCvtColor(m_imageHandle.get(), m_imageHandle.get(), CV_RGB2BGR);
	
	return cvSaveImage(filename.c_str(), m_imageHandle.get());
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
				cvFlip(m_imageHandle.get(), 0);
				cvCvtColor(image, m_imageHandle.get(), CV_RGB2BGR);
				saved = cvSaveImage(filename.c_str(), m_imageHandle.get());
			}
			else if(4 == image->nChannels)
			{
				cvFlip(m_imageHandle.get(), 0);
				cvCvtColor(image, m_imageHandle.get(), CV_RGBA2BGRA);
				saved = cvSaveImage(filename.c_str(), m_imageHandle.get());
			}
		}
		else
		{
			cvFlip(m_imageHandle.get(), 0);
			saved = cvSaveImage(filename.c_str(), image);
		}
	}
	
	return saved;
}

bool ImageIO::saveTexture(const string &filename, Texture &texture)
{
  return saveTexture(filename, &texture);
}

bool ImageIO::saveTexture(const string &filename, Texture *texture)
{
	ensureImageSize(texture->getWidth(), texture->getHeight(), texture->getChannelCount());
	
	bool saved = false;
	if(GL_UNSIGNED_BYTE == texture->getDataType())
	{
		texture->transferFromTexture(m_imageHandle.get());
		
		bool reorderChannels = texture->getFormat() == GL_RGBA || texture->getFormat() == GL_RGB;
		cvFlip(m_imageHandle.get(), 0);
		saved = saveImage(filename, m_imageHandle.get(), reorderChannels);
	}
	else if(GL_FLOAT == texture->getDataType())
	{
	  shared_ptr<IplImage> floatImage = shared_ptr<IplImage>(
		cvCreateImage(cvSize(texture->getWidth(), texture->getHeight()), IPL_DEPTH_32F, 3),
		[](IplImage* ptr){ cvReleaseImage(&ptr); });
	  texture->transferFromTexture(floatImage.get());
	  cvSave(filename.c_str(), floatImage.get()); 
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
		cvFlip(m_imageHandle.get(), 0);
        cvCvtColor(image, image, CV_BGR2RGB);
	}

	return image;
}

void ImageIO::ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight, const unsigned int channelCount)
{
	//	Check and see if the image is the correct size. If it is do nothing
	if (m_imageWidth != imageWidth || m_imageHeight != imageHeight || (unsigned int)m_imageHandle->nChannels != channelCount)
	{
		m_imageHandle = shared_ptr<IplImage>(
		  cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, channelCount),
		  [](IplImage* ptr){ cvReleaseImage(&ptr); });

		m_imageWidth = imageWidth;
		m_imageHeight = imageHeight;
	}
}
