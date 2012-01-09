#include "HolovideoEncoder.h"

HolovideoEncoder::HolovideoEncoder()
{
  m_filename  = "";
}

void HolovideoEncoder::initCodec(string& filename, int width, int height)
{
  setWidth(width);
  setHeight(height);
  m_filename  = filename;
}

void HolovideoEncoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  //  Open the stream to write to
  if(NULL == glWidget)
  {
	//	No OpenGL encoding widget. Cant open encoding stream
	return;
  }

  m_glWidget = glWidget;
  m_glWidget->setEncodingContext(&m_encoder);
  m_glWidget->setGLContext(&m_encoder);
  m_glWidget->reinit(getWidth(), getHeight());

  m_io.openSaveStream(m_filename, getWidth(), getHeight(), 30);
}

void HolovideoEncoder::closeCodec(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void HolovideoEncoder::process(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(data);

  MeshInterchange* mesh = m_glWidget->encode();

  //  Encode to the stream
  m_io.saveStream(*(mesh->getTexture()));
}

void HolovideoEncoder::previewProcess(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(data);

  //  Dont want to encode, only draw
  m_glWidget->updateScene();
}

string HolovideoEncoder::codecName(void)
{
  return "Holovideo Encoder";
}
