/*
 *  RenderTextureFacade.h
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#ifndef _FBO_FACADE_H_
#define _FBO_FACADE_H_

#include <QtOpenGL/QGLWidget>

#include "TextureFacade.h"
#include "OGLStatus.h"

class FBOFacade
{
private:
	GLuint m_renderingQuad;
	
	GLuint m_frameBuffer;	//	FBO identifier
public:
	FBOFacade(void);
	~FBOFacade();
	
	bool init();

	void bind(void);
	void unbind(void);
	void bindDrawBuffer(GLenum attachmentPoint);
	void setTextureAttachPoint(const TextureFacade &texture, GLenum attachmentPoint);	
	void process(void);
	
private:
	void _initFBO(void);
	void _cacheQuad(void);
};

#endif