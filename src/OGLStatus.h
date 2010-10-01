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

#include <QtOpenGL/QGLWidget>

#include "Logger.h"

using namespace std;

class OGLStatus
{
public:
	static void logOGLErrors (const string &label);
	static bool logFBOStatus(void);
};

#endif	// _OGL_STATUS_H_