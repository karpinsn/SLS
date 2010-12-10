/*
 Filename:		MainWindowView.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	10/07/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 10/07/10 - Nik Karpinsky - Split MainWindow into controller and view classes
*/

#ifndef _MAIN_WINDOW_VIEW_H_
#define _MAIN_WINDOW_VIEW_H_

#include <iostream>
#include <QtGui/QWidget>
#include <qfiledialog.h>
#include <QListWidgetItem>
#include <QProgressDialog>

//	Qt auto generated headers
#include "ui_Holoencoder.h"

#include <wrench/gl/Camera.h>
#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "Holodecoder.h"
#include "ImageIO.h"
#include "AbstractMesh.h"
#include "XYZFileIO.h"

#include "MainWindowController.h"

class MainWindowController;

using namespace wrench;

class MainWindowView : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

	friend class MainWindowController;
public:
	MainWindowView(QMainWindow* parent);
	~MainWindowView();
	
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

#endif	// _MAIN_WINDOW_VIEW_H_
