/*
 *  gl::wrench::VBO.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "VAO.h"

wrench::gl::VAO::VAO(void)
{
    m_vaoID = 0;
    m_indexCount = 0;
}

wrench::gl::VAO::~VAO()
{
    if(0 != m_vaoID)
    {
        glDeleteVertexArrays(1, &m_vaoID);
    }
}

bool wrench::gl::VAO::init(GLenum mode, GLsizei count)
{
    m_mode = mode;
    m_count = count;

    glGenVertexArrays(1, &m_vaoID);

    OGLStatus::logOGLErrors("wrench::gl::VAO - init()");
    return (0 != m_vaoID);
}

void wrench::gl::VAO::addVBO(VBO& vbo, std::string attributeName)
{
    GLuint attributeAddress = (GLuint)m_vertexAttributes.size();

    _bind();
    vbo.bind();
    glVertexAttribPointer(attributeAddress, vbo.getComponentSize(), vbo.getComponentType(), GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attributeAddress);

    m_vertexAttributes.insert(pair<std::string, int>(attributeName, attributeAddress));
    _unbind();

    OGLStatus::logOGLErrors("wrench::gl::VAO - addVBO()");
}

void wrench::gl::VAO::addIBO(IBO& ibo)
{
  _bind();

  m_indexCount = ibo.getComponentCount();

  ibo.bind();
  _unbind();
}

GLuint wrench::gl::VAO::getVertexAttributeAddress(std::string attributeName)
{
    return (GLuint)m_vertexAttributes[attributeName];
}


void wrench::gl::VAO::draw()
{
    //  Bind the VAO
    _bind();

    if(m_indexCount > 0)  // If we have an IBO then we need to draw using draw elements
    {
      glDrawElements(m_mode, m_indexCount, GL_UNSIGNED_INT, 0);
    }
    else // Otherwise its just VBOs and draw the array
    {
      glDrawArrays(m_mode, 0, m_count);
    }
    _unbind();

    OGLStatus::logOGLErrors("wrench::gl::VAO - draw()");
}

void wrench::gl::VAO::_bind()
{
    glBindVertexArray(m_vaoID);
    OGLStatus::logOGLErrors("wrench::gl::VAO - _bind()");
}

void wrench::gl::VAO::_unbind()
{
    glBindVertexArray(0);
    OGLStatus::logOGLErrors("wrench::gl::VAO - _unbind()");
}
