/**
@file
@author Nikolaus Karpinsky
@since  09/01/09
*/

#ifndef _ABSTRACT_GL_CONTEXT_H_
#define _ABSTRACT_GL_CONTEXT_H_

using namespace std;

class IGLContext
{
public:
	virtual ~IGLContext(void) {};

	virtual void init(void) = 0;
    virtual void init(int width, int height) = 0;
	virtual void draw(void) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void cameraSelectMode(int mode) = 0;
	virtual void mousePressEvent(int mouseX, int mouseY) = 0;
	virtual void mouseMoveEvent(int mouseX, int mouseY) = 0;
};

#endif	// _ABSTRACT_GL_CONTEXT_H_
