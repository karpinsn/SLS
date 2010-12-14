/*
 Filename:		OGLStatus.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _OGL_STATUS_H_
#define _OGL_STATUS_H_

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

using namespace std;

namespace wrench
{
	namespace gl 
	{
		class OGLStatus
		{
		public:
			static void logOGLErrors (const string &label);
			static bool logFBOStatus(void);
		};
	}
}

#endif	// _OGL_STATUS_H_
