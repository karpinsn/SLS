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
    m_vboId = 0;
}

wrench::gl::VBO::~VBO()
{
    glDeleteFramebuffersEXT(1, &m_vboId);
}

bool wrench::gl::VBO::init()
{
	_cacheQuad();
	_initFBO();
	
        OGLStatus::logOGLErrors("wrench::gl::VBO - init()");
	
	return true;
}

void wrench::gl::VBO::draw()
{
    //  Bind the VBO
    _bind();

    glEnableClientState(GL_VERTEX_ARRAY);

}

void wrench::gl::VBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_framebuffer);
        OGLStatus::logOGLErrors("wrench::gl::VBO - bind()");
}

void wrench::gl::VBO::process(void)
{
	glPushAttrib(GL_VIEWPORT_BIT);
	{
		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity ();
		gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();
		glViewport (0, 0, 512, 512);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT,GL_FILL);
	
		glColor3f(1.0f, 1.0f, 1.0f);
		glCallList(m_renderingQuad);
	
		//	Pop back matricies as if nothing happened
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopAttrib();
		
        OGLStatus::logOGLErrors("wrench::gl::VBO - process()");
}

void wrench::gl::FBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);
	
        OGLStatus::logOGLErrors("wrench::gl::VBO - unbind()");
}

void wrench::gl::FBO::bindDrawBuffer(GLenum attachmentPoint)
{
	glDrawBuffer(attachmentPoint);
}

void wrench::gl::FBO::setTextureAttachPoint(const Texture &texture, const GLenum attachmentPoint)
{
	bind();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachmentPoint, texture.getTextureTarget(), texture.getTextureId(), 0);
}

void wrench::gl::FBO::_cacheQuad(void)
{
	//	Quad used to render image operations
	m_renderingQuad = glGenLists(1);
	
	glNewList(m_renderingQuad,GL_COMPILE);
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin (GL_QUADS);
		{
			glTexCoord2f (0.0f,0.0f); /* lower left corner of image */
			glVertex3f (-1.0f, -1.0f, 0.0f);
			glTexCoord2f (1.0f, 0.0f); /* lower right corner of image */
			glVertex3f (1.0f, -1.0f, 0.0f);
			glTexCoord2f (1.0f, 1.0f); /* upper right corner of image */
			glVertex3f (1.0f, 1.0f, 0.0f);
			glTexCoord2f (0.0f, 1.0f); /* upper left corner of image */
			glVertex3f (-1.0f, 1.0f, 0.0f);
		}
		glEnd ();
	}
	glEndList();
}

void wrench::gl::FBO::_initFBO(void)
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_framebuffer);

        OGLStatus::logOGLErrors("wrench::gl::VBO - _initFBO()");
}
