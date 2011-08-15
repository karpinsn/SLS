/*
 *  gl::wrench::VBO.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "VBO.h"

wrench::gl::VBO::VBO(void)
{
  m_vboID = 0;
}

wrench::gl::VBO::~VBO()
{
  if(0 != m_vboID)
  {
    glDeleteFramebuffersEXT(1, &m_vboID);
  }
}

bool wrench::gl::VBO::init(GLint componentSize, GLenum componentType, GLenum target)
{
  m_componentSize = componentSize;
  m_componentType = componentType;
  m_target = target;

  //  Generate a buffer on the GPU for the VBO
  glGenBuffers(1, &m_vboID);

  //  Log any errors that were encountered
  OGLStatus::logOGLErrors("wrench::gl::VBO - init()");
  return (0 != m_vboID);
}

void wrench::gl::VBO::bufferData(GLsizei size, const GLvoid* data, GLenum usage)
{
  //  Must bind the VBO before data can be buffered
  bind();
  //  Transfer the data to the GPU
  glBufferData(m_target, size * m_componentSize * Converter::typeToSize(m_componentType), data, usage);
  //  Log any OpenGL errors that were encountered
  OGLStatus::logOGLErrors("wrench::gl::VBO - bufferData()");
}

GLint wrench::gl::VBO::getComponentSize(void)
{
  return m_componentSize;
}

GLenum wrench::gl::VBO::getComponentType(void)
{
  return m_componentType;
}

GLenum wrench::gl::VBO::getTarget(void)
{
  return m_target;
}

void wrench::gl::VBO::bind()
{
  //  Bind the buffer and then log any OpenGL errors
  glBindBuffer(m_target, m_vboID);
  OGLStatus::logOGLErrors("wrench::gl::VBO - _bind()");
}

void wrench::gl::VBO::unbind()
{
  //  Unbind the buffer and then log any OpenGL errors
  glBindBuffer(m_target, 0);
  OGLStatus::logOGLErrors("wrench::gl::VBO - _unbind()");
}
