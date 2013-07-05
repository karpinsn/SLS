#include "Application.h"

int main(int argc, char **argv)
{
	// Create the Qt application
	QApplication app(argc, argv);
	// Need to force the loading of the resources since it is in the SLS library
	Q_INIT_RESOURCE(MainWindow);

	//	Set the stylesheet for the application
	// String defined in MainWindow.qss
	QFile file(":stylesheets/resources/SLS.qss");
	if( file.open(QFile::ReadOnly) )
	{
	  QString stylesheet = QLatin1String(file.readAll());
	  app.setStyleSheet(stylesheet);
	}

	//	Set the application specific info
    QCoreApplication::setOrganizationName("SL Studios");
    QCoreApplication::setOrganizationDomain("SLStudios.org");
    QCoreApplication::setApplicationName("SLS-Desktop");
	
	// Create a main widget and start it up
	MainWindowController* mainWindowController = new MainWindowController();
	mainWindowController->showWidget();
	
	//	Set the logger level
	wrench::Logger::setLogLevel( wrench::Logger::Error );

	// Start up the application run loop
	return app.exec();
}
