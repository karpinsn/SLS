/*
 *  Logger.h
 *  Holoencoder
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>

using namespace std;

class Logger
{
public:
	static void logError(const string &message);
	static void logDebug(const string &message);
};

#endif