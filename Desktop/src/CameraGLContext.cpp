#include "CameraGLContext.h"

CameraGLContext::CameraGLContext(void)
{
}

void CameraGLContext::setBuffer(ImageBuffer *buffer)
{
  m_buffer = buffer;
}

void CameraGLContext::init()
{
  m_frontBufferIndex = 0;
  _cacheQuad();
  _initShaders();
  _initTextures(640, 480);
}

void CameraGLContext::_initShaders(void)
{
  m_textureDisplay.init();
  m_textureDisplay.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/TextureDisplay.vert"));
  m_textureDisplay.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/TextureDisplay.frag"));
  m_textureDisplay.bindAttributeLocation("vert", 0);
  m_textureDisplay.bindAttributeLocation("vertTexCoord", 1);

  m_textureDisplay.link();
  m_textureDisplay.uniform("texture", 0);
  OGLStatus::logOGLErrors("CameraGLContext - _initShaders()");
}

void CameraGLContext::_initTextures(GLuint width, GLuint height)
{
  m_frame0.init(width, height, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
  m_frame1.init(width, height, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE);
  m_textureBuffer[0] = &m_frame0;
  m_textureBuffer[1] = &m_frame1;
}

void CameraGLContext::draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  {
	  OGLStatus::logOGLErrors("CameraGLContext - draw()");
    m_textureDisplay.bind();
    {
      m_textureBuffer[m_frontBufferIndex]->bind(GL_TEXTURE0);
      m_screen.draw();
    }
    m_textureDisplay.unbind();
  }
  glPopMatrix();
  OGLStatus::logOGLErrors("CameraGLContext - draw()");
}

void CameraGLContext::resize(int width, int height)
{
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void CameraGLContext::cameraSelectMode(int mode)
{

}

void CameraGLContext::mousePressEvent(int mouseX, int mouseY)
{

}

void CameraGLContext::mouseMoveEvent(int mouseX, int mouseY)
{

}

void CameraGLContext::setBackBuffer(IplImage* image)
{
    int backBufferIndex = (m_frontBufferIndex + 1) % 2;
    m_textureBuffer[backBufferIndex]->transferToTexture(image);

    //	Make sure we dont have any errors
    OGLStatus::logOGLErrors("CameraGLContext - setBackBuffer()");
}

void CameraGLContext::swapBuffers(void)
{
    //	Switch the front and back buffer
    m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;

    //	Make sure we dont have any errors
    OGLStatus::logOGLErrors("CameraGLContext - swapBuffers()");
}

void CameraGLContext::_cacheQuad(void)
{
  float* vertex   = new float[12];	// vertex array
  float* tex      = new float[8];         // texture coord array

  vertex[0] = -1.0f; vertex[1]   =-1.0f; vertex[2]  = 0.0f;
  vertex[3] =  1.0f; vertex[4]   =-1.0f; vertex[5]  = 0.0f;
  vertex[6] =  1.0f; vertex[7]   = 1.0f; vertex[8]  = 0.0f;
  vertex[9] = -1.0f; vertex[10]  = 1.0f; vertex[11] = 0.0f;

  tex[0] = 0.0f; tex[1] = 0.0f;
  tex[2] = 1.0f; tex[3] = 0.0f;
  tex[4] = 1.0f; tex[5] = 1.0f;
  tex[6] = 0.0f; tex[7] = 1.0f;

  m_screen.init(GL_QUADS, 4);
  m_vertex.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
  m_vertex.bufferData(4, vertex, GL_STATIC_DRAW);
  m_screen.addVBO(m_vertex, "vert");

  m_texCoords.init(2, GL_FLOAT, GL_ARRAY_BUFFER);
  m_texCoords.bufferData(4, tex, GL_STATIC_DRAW);
  m_screen.addVBO(m_texCoords, "vertTexCoord");

  delete [] vertex;
  delete [] tex;
}
