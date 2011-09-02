#include "MultiWavelengthCodec.h"

void MultiWavelengthCodec::openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height)
{
  //  Open the stream to write to
  m_io.openSaveStream(filename, width, height, 30);
}

void MultiWavelengthCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
  m_io.saveStream(data);
}

void MultiWavelengthCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void MultiWavelengthCodec::openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename)
{
  if(NULL == glWidget)
  {
    //  No OpenGL encoding widget. Cant open decode stream
    return;
  }

  m_io.openReadStream(filename);
  m_calculateReference = true;
  m_glWidget = glWidget;

  m_glWidget->setGLContext(&m_coder);
  m_glWidget->reinit(480, 480); //  TODO comeback and fix this
}

MeshInterchange* MultiWavelengthCodec::decode()
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return an empty MeshInterchange
    return NULL;
  }

  if(_streamUntilNewFrame())
  {
    //  End of the file. Return an empty mesh
    return NULL;
  }

  if(m_calculateReference)
  {
	//	Capture the reference plane
    m_calculateReference = false;
    m_coder.captureReferencePlane();
	m_glWidget->encode();

	//	Stream again until we get an actual frame
	_streamUntilNewFrame();
  }

  m_glWidget->encode();

  return new MeshInterchange(&m_coder.m_depthMap);
}

void MultiWavelengthCodec::closeDecodeStream(void)
{
  m_glWidget = NULL;
}

bool MultiWavelengthCodec::_streamUntilNewFrame(void)
{
  IplImage* frame = m_io.readStream();

  //  While the frame is not null
  //  and we dont have a new 3D frame
  while(NULL != frame && !m_coder.newImage(frame))
  {
    frame = m_io.readStream();
  }

  return frame == NULL;
}