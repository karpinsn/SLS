/*
 Filename:		AbstractGLContext.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _ABSTRACT_GL_CONTEXT_H_
#define _ABSTRACT_GL_CONTEXT_H_

using namespace std;

class AbstractGLContext
{
public:
	virtual ~AbstractGLContext(void) {};

	virtual void init(void) = 0;
	virtual void draw(void) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void cameraSelectMode(int mode);
	virtual void mousePressEvent(int mouseX, int mouseY) = 0;
	virtual void mouseMoveEvent(int mouseX, int mouseY) = 0;
};

#endif	// _ABSTRACT_GL_CONTEXT_H_