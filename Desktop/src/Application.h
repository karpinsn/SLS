/**
 @file
 @author		Nikolaus Karpinsky
 */

#ifndef _SLS_APPLICATION_H_
#define _SLS_APPLICATION_H_

//	Needed so that windows.h does not include Winsock.h
#ifdef _WIN32
	#include <WinSock2.h>
	#include <windows.h>
#endif

#include <QtGui/QApplication>
#include "MainWindowController.h"

#endif	// _SLS_APPLICATION_H_
