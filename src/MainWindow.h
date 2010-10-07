/*
 Filename:		MainWindow.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <iostream>
#include <QtGui/QWidget>
#include <qfiledialog.h>
#include <QListWidgetItem>
#include <QProgressDialog>

//	Qt auto generated headers
#include "ui_Holoencoder.h"

#include "Camera.h"
#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "Holodecoder.h"
#include "ImageIO.h"
#include "AbstractMesh.h"
#include "XYZFileIO.h"

class MainWindowController;
class MainWindow;

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
	MainWindow* m_mainWindow;
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

	friend class MainWindowController;
public:
	MainWindow(QMainWindow* parent);
	~MainWindow();
	
	void glInit(void);
	
	// Connect up the signals with the controller
	void connectSignalsWithController(QObject* controller);
	
private:
	OpenGLWidget* m_glWidget;
	OpenGLWidget* m_viewerWidget;
	
	Holoencoder* m_holoEncoder;
	Holodecoder* m_holoDecoder;
	
private:
	void _initTopMenu(void);
};

#endif	// _MAIN_WINDOW_H_
