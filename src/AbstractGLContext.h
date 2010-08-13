#pragma once

using namespace std;

class AbstractGLContext
{
public:
	virtual ~AbstractGLContext(void) {};

	virtual void init(void) = 0;
	virtual void draw(void) = 0;
	virtual void resize(int width, int height) = 0;
	virtual void mousePressEvent(int mouseX, int mouseY) = 0;
	virtual void mouseMoveEvent(int mouseX, int mouseY) = 0;
};