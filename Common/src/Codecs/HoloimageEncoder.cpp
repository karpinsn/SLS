#include "HoloimageEncoder.h"

HoloimageEncoder::HoloimageEncoder()
{
  m_filename  = "";
}

HoloimageEncoder::~HoloimageEncoder()
{
  cvReleaseImage(&m_image);
}

void HoloimageEncoder::initCodec(string& filename, int width, int height)
{
  setWidth(width);
  setHeight(height);
  m_filename  = filename;
}

void HoloimageEncoder::openCodec(EncodingOpenGLWidget* glWidget)
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
}

void HoloimageEncoder::closeCodec(void)
{
}

void HoloimageEncoder::process(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(data);

  MeshInterchange* mesh = m_glWidget->encode();
  mesh->getTexture()->transferFromTexture(m_image);
}

void HoloimageEncoder::previewProcess(MeshInterchange* data)
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

void HoloimageEncoder::autoFitData(void)
{
  m_encoder.autoFitTransforms();

  if(NULL != m_glWidget)
  {
	m_glWidget->updateScene();
  }
}

string HoloimageEncoder::codecName(void)
{
  return "Holoimage Encoder";
}
