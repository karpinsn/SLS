#include "MeshInterchange.h"

bool MeshInterchange::setData(IplImage* image)
{
  m_image = image;
  return true;
  /*
  bool successful = false;

  //  If there is more or less than 3 channels then just fail
  if(image->nChannels != 3)
  {
    _checkClearData();
    m_width   = image->width;
    m_height  = image->height;

    m_data = new glm::vec3[m_width * m_height];

    //  Actual data transfer
    for (int i = 0; i < m_height; i++)
    {
        //  OpenCV does not guarentee continous memory blocks so it has to be copied row by row
        memcpy(m_data + (i * m_width), image->imageData + (i * image->widthStep), m_width * image->nChannels * sizeof(float));
    }
  }

  return successful;*/
}

bool MeshInterchange::setData(Texture* texture)
{
  m_texture = texture; return true;
  /*
  bool successful = false;

  //  If there is more or less than 3 channels then just fail
  if(3 != texture->getChannelCount())
  {
    _checkClearData();
    m_width   = texture->getWidth();
    m_height  = texture->getHeight();

    m_data = new glm::vec3[m_width * m_height];

    //  TODO come back and fix this
    IplImage* image = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_32F, 3);
    texture->transferFromTexture(image);

    //  Actual data transfer
    for (int i = 0; i < m_height; i++)
    {
        //  OpenCV does not guarentee continous memory blocks so it has to be copied row by row
        memcpy(m_data + (i * m_width), image->imageData + (i * image->widthStep), m_width * image->nChannels * sizeof(float));
    }
  }

  return successful;*/
}

const glm::vec3* MeshInterchange::getDataPointer()
{
  return m_data;
}

int MeshInterchange::getHeight()
{
  return m_height;
}

int MeshInterchange::getWidth()
{
  return m_width;
}

void MeshInterchange::_checkClearData(void)
{
  if(NULL != m_data)
  {
    delete [] m_data;
  }
}
