/*
 Filename:		OGLStatus.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include <iostream>
#include <sstream>

#ifdef __APPLE__
	#include <glew.h>
	#include <OpenGL/gl.h>
#elif _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include "../Logger.h"

namespace wrench
{
	namespace gl 
	{
                class Converter
		{
		public:
                        static size_t typeToSize (const GLenum);
                };
	}
}

#endif	// _CONVERTER_H_
