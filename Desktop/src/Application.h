/*
 Filename:		MainWindowController.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	10/07/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 10/07/10 - Nik Karpinsky - Split MainWindow into controller and view classes
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
