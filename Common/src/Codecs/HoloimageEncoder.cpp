#include "HoloimageEncoder.h"

HoloimageEncoder::HoloimageEncoder()
{
  m_filename = "";
  m_frameNumber = 0;
}

void HoloimageEncoder::initCodec(string& filename, int width, int height)
{
  m_width = width;
  m_height = height;
  m_filename = filename;
}

void HoloimageEncoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  //  Open the stream to write to
  if(nullptr == glWidget)
  {
	//	No OpenGL encoding widget. Cant open encoding stream
	return;
  }

  m_glWidget = glWidget;
  //  Need to set the size on the coder and our OpenGL widget
  m_encoder.init(getWidth(), getHeight());
  m_glWidget->setEncodingContext(&m_encoder);
  m_glWidget->reinit(getWidth(), getHeight());
}

void HoloimageEncoder::closeCodec(void)
{
}

shared_ptr<MeshInterchange> HoloimageEncoder::process(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return nullptr;
  }

  m_encoder.setCurrentMesh(data);

  MeshInterchange* mesh = m_glWidget->encode();

  //  Format the filename and write the image out
  sprintf(m_filenameBuffer, m_filename.c_str(), m_frameNumber++);
  m_io.saveTexture(m_filenameBuffer, mesh->getTexture());

  return nullptr;
}

shared_ptr<MeshInterchange> HoloimageEncoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return nullptr;
  }

  m_encoder.setCurrentMesh(data);

  //  Dont want to encode, only draw
  m_glWidget->previewEncode();

  return nullptr;
}

void HoloimageEncoder::autoFitData(void)
{
  m_encoder.autoFitTransforms();

  if(nullptr != m_glWidget)
  {
	m_glWidget->updateScene();
  }
}

string HoloimageEncoder::codecName(void)
{
  return "Holoimage Encoder";
}
