#ifndef _OGLSTATUS_H_
#define _OGLSTATUS_H_

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

#endif