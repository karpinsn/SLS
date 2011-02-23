/*
 *  Logger.cpp
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "Logger.h"

void Logger::logError(const string &message)
{
	clog << "Error: " << message << endl;
}

void Logger::logDebug(const string &message)
{
	clog << "Debug: " << message << endl;
}
