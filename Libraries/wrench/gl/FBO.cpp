/*
 *  RenderTextureFacade.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "FBO.h"

wrench::gl::FBO::FBO(void)
{
}

wrench::gl::FBO::~FBO()
{
  glDeleteFramebuffersEXT(1, &m_framebuffer);
}

bool wrench::gl::FBO::init(int width, int height)
{
  m_width = width;
  m_height = height;

  _cacheQuad();
  _initFBO();

  OGLStatus::logOGLErrors("FBOFacade - init()");

  return true;
}

bool wrench::gl::FBO::reinit(int width, int height)
{
  m_width = width;
  m_height = height;

  glDeleteFramebuffersEXT(1, &m_framebuffer);
  _initFBO();

  OGLStatus::logOGLErrors("FBOFacade - init()");

  return true;
}

void wrench::gl::FBO::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_framebuffer);

  OGLStatus::logOGLErrors("FBOFacade - bind()");
}

void wrench::gl::FBO::process(void)
{
  glPushAttrib(GL_VIEWPORT_BIT);
  {
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity ();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glViewport (0, 0, m_width, m_height);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT,GL_FILL);

    m_screen.draw();

    //	Pop back matricies as if nothing happened
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  }
  glPopAttrib();

  OGLStatus::logOGLErrors("FBOFacade - process()");
}

void wrench::gl::FBO::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
  glDrawBuffer(GL_BACK);

  OGLStatus::logOGLErrors("FBOFacade - unbind()");
}

void wrench::gl::FBO::bindDrawBuffer(GLenum attachmentPoint)
{
  glDrawBuffer(attachmentPoint);
}

void wrench::gl::FBO::setTextureAttachPoint(const Texture &texture, const GLenum attachmentPoint)
{
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentPoint, texture.getTextureTarget(), texture.getTextureId(), 0);
}

void wrench::gl::FBO::_cacheQuad(void)
{	
  float* vertex   = new float[12];	// vertex array
  float* tex      = new float[8];         // texture coord array

  vertex[0] = -1.0f; vertex[1]   =-1.0f; vertex[2]  = -1.0f;
  vertex[3] =  1.0f; vertex[4]   =-1.0f; vertex[5]  = -1.0f;
  vertex[6] =  1.0f; vertex[7]   = 1.0f; vertex[8]  = -1.0f;
  vertex[9] = -1.0f; vertex[10]  = 1.0f; vertex[11] = -1.0f;

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

void wrench::gl::FBO::_initFBO(void)
{
  glGenRenderbuffersEXT(1, &m_rbo);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_rbo);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                           m_width, m_height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_framebuffer);

  // attach the renderbuffer to depth attachment point
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                               GL_RENDERBUFFER_EXT, m_rbo);


  OGLStatus::logOGLErrors("FBOFacade - _initFBO()");
}
