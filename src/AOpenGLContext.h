#ifndef _AOpenGLContext_H
#define _AOpenGLContext_H

class AOpenGLContext
{
public:	
    virtual void init() = 0;
	virtual void draw(void) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void mousePressEvent(int mouseX, int mouseY) = 0;
	virtual void mouseMoveEvent(int mouseX, int mouseY) = 0;
};

#endif
