#include "ImageIO.h"

bool ImageIO::loadOpenGLTexture2D(const string &filename, GLuint *textureName, GLint internalFormat)
{
	ILuint texid = 0;
	ilGenImages(1, &texid);
	ilBindImage(texid);
	bool success = ilLoadImage(filename.c_str());
	
	if (success)
	{
		success = ilConvertImage(internalFormat, IL_UNSIGNED_BYTE);
		if (success)
		{
			//	Generate and bind the texture
			glGenTextures(1, textureName); 
			glBindTexture(GL_TEXTURE_2D, *textureName);
			
			//	Use linear interpolation and then generate the mipmap
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
			gluBuild2DMipmaps(GL_TEXTURE_2D, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
							  ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), 
							  GL_UNSIGNED_BYTE, ilGetData());
		}
	}
	
	//	Since the image is a texture we can now delete the image
	ilDeleteImages(1, &texid);
	
	return success;
}

bool ImageIO::saveRGBImage(const string &filename, GLvoid *imageData, unsigned int imageWidth, unsigned int imageHeight)
{
	ILuint texid = 0;
	ilGenImages(1, &texid);
	ilBindImage(texid);

	bool success = ilTexImage(imageWidth, imageHeight, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, imageData);
	if (success)
	{
		success = ilSetData(imageData);
		if (success)
		{
			success = ilSaveImage(filename.c_str());
		}
	}
	
	return success;
}

bool ImageIO::saveGrayScaleImage(const string &filename, unsigned char *imageData, unsigned int imageWidth, unsigned int imageHeight)
{
	unsigned char grayScaleData[imageHeight * imageWidth * 3];
	for (unsigned int pixel = 0; pixel < imageHeight * imageWidth; pixel++)
	{
		grayScaleData[pixel * 3] = imageData[pixel];
		grayScaleData[pixel * 3 + 1] = imageData[pixel];
		grayScaleData[pixel * 3 + 2] = imageData[pixel];
	}
	
	return saveRGBImage(filename, grayScaleData, imageWidth, imageHeight);
}