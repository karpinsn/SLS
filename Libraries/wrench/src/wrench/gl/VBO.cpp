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

    glGenBuffers(1, &m_vboID);

    OGLStatus::logOGLErrors("wrench::gl::VBO - init()");
    return (0 != m_vboID);
}

void wrench::gl::VBO::bufferData(GLsizei size, const GLvoid* data, GLenum usage)
{
   bind();
   glBufferData(m_target, size * m_componentSize * Converter::typeToSize(m_componentType), data, usage);

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
  glBindBuffer(m_target, m_vboID);
  OGLStatus::logOGLErrors("wrench::gl::VBO - _bind()");
}

void wrench::gl::VBO::unbind()
{
  glBindBuffer(m_target, 0);
  OGLStatus::logOGLErrors("wrench::gl::VBO - _unbind()");
}
