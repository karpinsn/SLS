#include "CameraGLContext.h"

CameraGLContext::CameraGLContext(void)
{
  m_hasBeenInit = false;
  m_imageWidth = 0.0;
  m_imageHeight = 0.0;
}

void CameraGLContext::init()
{
  if(!m_hasBeenInit)
  {
    m_imageWidth = 256.0f;
    m_imageHeight = 256.0f;

    m_frontBufferIndex = 0;
    _cacheQuad();
    _initShaders();
    _initTextures((GLuint)m_imageWidth, (GLuint)m_imageHeight);
    m_hasBeenInit = true;
  }
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
  m_frame0.init(width, height, GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE);
  m_frame1.init(width, height, GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE);
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

void CameraGLContext::draw(Texture* texture)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  {
    OGLStatus::logOGLErrors("CameraGLContext - draw(Texture)");
    m_textureDisplay.bind();
    {
      texture->bind(GL_TEXTURE0);
      m_screen.draw();
    }
    m_textureDisplay.unbind();
  }
  glPopMatrix();
  OGLStatus::logOGLErrors("CameraGLContext - draw(Texture)");
}

void CameraGLContext::resizeInput(float width, float height)
{
  //  Make sure that it has been initalized first.
  if(m_hasBeenInit)
  {
    m_imageWidth = width;
    m_imageHeight = height;

    m_frame0.init((GLuint)width, (GLuint)height, GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE);
    m_frame1.init((GLuint)width, (GLuint)height, GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE);
  }
}

void CameraGLContext::resize(int width, int height)
{
  //  Center the viewport
  int x = (width - m_imageWidth) / 2.0;
  int y = (height - m_imageHeight) / 2.0;
  glViewport(x, y, m_imageWidth, m_imageHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

void CameraGLContext::newImage(IplImage* image)
{
    int backBufferIndex = (m_frontBufferIndex + 1) % 2;
    m_textureBuffer[backBufferIndex]->transferToTexture(image);
    swapBuffers();
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
  float vertex[12]; // vertex array
  float tex[8];     // texture coord array

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
}
