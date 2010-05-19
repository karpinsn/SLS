#ifndef IMAGE_IO_HEADER
#define IMAGE_IO_HEADER

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
#endif

#include <string>
#include <IL/il.h>

using namespace std;

class ImageIO
{
public:
	bool loadOpenGLTexture2D(const string &filename, GLuint *textureName, GLint internalFormat);
	bool saveRGBImage(const string &filename, GLvoid *imageData, unsigned int imageWidth, unsigned int imageHeight);
	
	bool saveGrayScaleImage(const string &filename, unsigned char *imageData, unsigned int imageWidth, unsigned int imageHeight);
};

#endif // IMAGE_IO_HEADER