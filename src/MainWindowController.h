/*
 Filename:		MainWindowController.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	10/07/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 10/07/10 - Nik Karpinsky - Split MainWindow into controller and view classes
 */

#ifndef _MAIN_WINDOW_CONTROLLER_H_
#define _MAIN_WINDOW_CONTROLLER_H_

#include <iostream>
#include <QtGui/QWidget>
#include <qfiledialog.h>
#include <QListWidgetItem>
#include <QProgressDialog>

//	Qt auto generated headers
#include "ui_Holoencoder.h"

#include "MainWindowView.h"
#include "Camera.h"
#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "Holodecoder.h"
#include "ImageIO.h"
#include "AbstractMesh.h"
#include "XYZFileIO.h"

class MainWindowView;

class MainWindowController : public QObject
{
	Q_OBJECT
	
public:
	MainWindowController();
	~MainWindowController();
	
	void showWidget();
	
public slots:
	void onEncodeButton();
	void onOpenXYZM();
	void selectXYZM(QListWidgetItem* current, QListWidgetItem* previous);
	void playVideo(void);
	
	void toolSelect(const int tool);
	
private:
	MainWindowView* m_mainWindow;
};

#endif	// _MAIN_WINDOW_CONTROLLER_H_
