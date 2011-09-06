#include "VideoIO.h"

VideoIO::VideoIO(void)
{
  m_videoWriterInUse = false;
  m_videoReaderInUse = false;
  m_imageWidth = 1;
  m_imageHeight = 1;

  m_imageHandle = cvCreateImage(cvSize(m_imageWidth, m_imageHeight), IPL_DEPTH_8U, 3);
  m_floatImageHandle = cvCreateImage(cvSize(m_imageWidth, m_imageHeight), IPL_DEPTH_32F, 3);
}

VideoIO::~VideoIO()
{
  if (m_videoWriterInUse)
  {
    clog << "Video writer stream not properly closed. Closing video writer stream" << endl;
    cvReleaseVideoWriter(&m_videoWriterHandle);
  }
  cvReleaseImage(&m_imageHandle);   //  TODO comeback and fix this
}

bool VideoIO::openSaveStream(const string &filename, const unsigned int videoWidth, const unsigned int videoHeight, const unsigned int fps)
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

bool VideoIO::saveStream(MeshInterchange& mesh)
{
  bool successful = false;

  if(MeshInterchange::TEXTURE_FORMAT == mesh.getPreferedFormat())
  {
	return saveStream(*mesh.getTexture());
  }
  else if(MeshInterchange::VERTEX_FORMAT == mesh.getPreferedFormat())
  {
	return saveStream(mesh.getIplImage());
  }

  return successful;
}

bool VideoIO::saveStream(Texture& texture)
{
  if(!m_videoWriterInUse)
  {
    clog << "Unable to write frame out to file as no current video writer handle exists" << endl;
    return false;
  }

  ensureImageSize(texture.getWidth(), texture.getHeight(), texture.getChannelCount());

  if(GL_FLOAT == texture.getDataType())
  {
    //  Floating point texture that needs to be converted to 8 bit before being saved
    texture.transferFromTexture(m_floatImageHandle);
    cvConvertScale(m_floatImageHandle, m_imageHandle, 1.0/256.0);
  }
  else if(GL_UNSIGNED_BYTE == texture.getDataType())
  {
    texture.transferFromTexture(m_imageHandle);
  }
  else
  {
    clog << "Unable to write frame out to file. Unknown texture type" << endl;
    return false;
  }

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

  return m_videoWriterInUse;
}

bool VideoIO::saveStream(IplImage* image)
{
  if(!m_videoWriterInUse)
  {
    clog << "Unable to write frame out to file as no current video writer handle exists" << endl;
    return false;
  }

  cvWriteFrame(m_videoWriterHandle, image);

  return m_videoWriterInUse;
}

bool VideoIO::closeSaveStream(void)
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

bool VideoIO::openReadStream(const string &filename)
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

IplImage* VideoIO::readStream()
{
  IplImage *frame = NULL;

  if(m_videoReaderInUse)
  {
    frame = cvQueryFrame(m_videoReaderHandle);
    if(NULL != frame)
    {
      cvCvtColor(frame, frame, CV_BGR2RGB);
    }
  }
  else
  {
    clog << "Unable to read frame as no current video reader handle exists" << endl;
  }

  return frame;
}

bool VideoIO::closeReadStream(void)
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

bool VideoIO::readStreamIsOpen(void)
{
  return m_videoReaderInUse;
}

int VideoIO::readStreamWidth(void)
{
  if(!m_videoReaderInUse)
  {
	clog << "Unable to get width as no current video reader handle exists" << endl;
	return 0;
  }

  return cvGetCaptureProperty(m_videoReaderHandle, CV_CAP_PROP_FRAME_WIDTH);
}

int VideoIO::readStreamHeight(void)
{
  if(!m_videoReaderInUse)
  {
	clog << "Unable to get height as no current video reader handle exists" << endl;
	return 0;
  }

  return cvGetCaptureProperty(m_videoReaderHandle, CV_CAP_PROP_FRAME_HEIGHT);
}

float VideoIO::readStreamPosition(void)
{
  if(!m_videoReaderInUse)
  {
	clog << "Unable to get stream position as no current video reader handle exists" << endl;
	return 0.0;
  }

  return cvGetCaptureProperty(m_videoReaderHandle, CV_CAP_PROP_POS_AVI_RATIO);
}

void VideoIO::ensureImageSize(const unsigned int imageWidth, const unsigned int imageHeight, const unsigned int channelCount)
{
  //	Check and see if the image is the correct size. If it is do nothing
  if (m_imageWidth != imageWidth || m_imageHeight != imageHeight || (unsigned int)m_imageHandle->nChannels != channelCount)
  {
    cvReleaseImage(&m_imageHandle);
    cvReleaseImage(&m_floatImageHandle);
    m_imageWidth = imageWidth;
    m_imageHeight = imageHeight;

    m_imageHandle = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, channelCount);
    m_floatImageHandle = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_32F, channelCount);
  }
}

int VideoIO::_getTextureDepth(Texture& texture)
{
  int depth = -1;

  if(texture.getDataType() == GL_FLOAT)
  {
	depth = IPL_DEPTH_32F;
  }
  else if(texture.getDataType() == GL_UNSIGNED_BYTE)
  {
	depth = IPL_DEPTH_8U;
  }

  return depth;
}
