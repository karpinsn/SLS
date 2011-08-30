/*
 *  OGLStatus.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "Converter.h"

size_t wrench::gl::Converter::typeToSize(const GLenum type)
{
  size_t dataSize = 0;

  if(type == GL_FLOAT)
  {
    dataSize = sizeof(GLfloat);
  }
  else if(type == GL_UNSIGNED_BYTE)
  {
    dataSize = sizeof(GLubyte);
  }
  else if(type == GL_UNSIGNED_INT)
  {
    dataSize = sizeof(GLuint);
  }
  else
  {
    Logger::logError("wrench::gl::Converter - typeToSize() Invalid type specified");
  }

  return dataSize;
}