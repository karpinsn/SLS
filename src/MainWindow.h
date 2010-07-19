#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <iostream>
#include <QtGui/QWidget>
#include <qfiledialog.h>
#include <QListWidgetItem>

//	Qt auto generated headers
#include "ui_Holoencoder.h"

#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "ImageIO.h"

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
	
	// Connect up the signals with the controller
	void connectSignalsWithController(QObject* controller);
	
private:
	OpenGLWidget* m_glWidget;
	OpenGLWidget* m_viewerWidget;
	
	Holoencoder* m_holoEncoder;
	
	unsigned char* m_holoImage;
};

#endif
