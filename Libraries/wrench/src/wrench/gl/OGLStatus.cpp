/*
 *  OGLStatus.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "OGLStatus.h"

void wrench::gl::OGLStatus::logOGLErrors(const string &label) 
{
    GLenum errorCode;
    
    if ((errorCode = glGetError()) != GL_NO_ERROR) 
	{
		stringstream errorMessage;
		errorMessage << "OpenGL ERROR: " << gluErrorString(errorCode) << "(Message: " << label << ")";
		Logger::logError(errorMessage.str());
    }
}

bool wrench::gl::OGLStatus::logFBOStatus()
{
	bool fboComplete = false;

	switch(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) 
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			fboComplete = true;
			Logger::logDebug("Framebuffer complete!");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			Logger::logError("Framebuffer incomplete, incomplete attachment");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			Logger::logError("Unsupported framebuffer format");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			Logger::logError("Framebuffer incomplete, missing attachment");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			Logger::logError("Framebuffer incomplete, attached images must have same dimensions");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			Logger::logError("Framebuffer incomplete, attached images must have same format");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			Logger::logError("Framebuffer incomplete, missing draw buffer");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			Logger::logError("Framebuffer incomplete, missing read buffer");
			break;
	}
	
	return fboComplete;
}
