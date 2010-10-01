/*
 Filename:		FBOFacade.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
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

#endif	// _FBO_FACADE_H_