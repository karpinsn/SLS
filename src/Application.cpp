#include <QtGui/QApplication>

#include "MainWindow.h"

int main(int argc, char **argv)
{
	// Create the Qt application
	QApplication app(argc, argv);
	
	// Create a main widget and start it up
	MainWindowController* mainWindowController = new MainWindowController();
	mainWindowController->showWidget();
	
	// Start up the application run loop
	return app.exec();
}
