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
#include <QPropertyAnimation>

//	Qt auto generated headers
#include "ui_Holoencoder.h"

#include "MainWindowView.h"
#include <wrench/gl/Camera.h>
#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "Holodecoder.h"
#include "ImageIO.h"
#include "AbstractMesh.h"
#include "XYZFileIO.h"
#include <wrench/Logger.h>

class MainWindowView;

class MainWindowController : public QObject
{
	Q_OBJECT
	
public:
	MainWindowController();
	~MainWindowController();
	
	void showWidget();
	
        QAnimationGroup *animationGroup;
        QPropertyAnimation *fileListAnimation;
        QPropertyAnimation *mainWindowAnimation;

public slots:
	void exportSingleFrame();
	void exportEntireVideo();
	void onOpenXYZM();
	void selectXYZM(QListWidgetItem* current, QListWidgetItem* previous);
	void playVideo(void);
	void openHoloImage(void);
	
	void toolSelect(const int tool);

        void viewMode();
        void encodeMode();
	
private:
	MainWindowView* m_mainWindow;
};

#endif	// _MAIN_WINDOW_CONTROLLER_H_
