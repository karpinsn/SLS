#include "Application.h"

int main(int argc, char **argv)
{
	// Create the Qt application
	QApplication app(argc, argv);

	//	Set the stylesheet for the application
	QFile File(":/../../resources/images/resources/SLS.qss");
	File.open(QFile::ReadOnly);
	QString stylesheet = QLatin1String(File.readAll());
	app.setStyleSheet(stylesheet);

	//	Set the application specific info
    QCoreApplication::setOrganizationName("SL Studios");
    QCoreApplication::setOrganizationDomain("SLStudios.org");
    QCoreApplication::setApplicationName("SLS-Desktop");
	
	// Create a main widget and start it up
	MainWindowController* mainWindowController = new MainWindowController();
	mainWindowController->showWidget();
	
	//	Set the logger level
	wrench::Logger::setLogLevel(wrench::LogLevel::Error);

	// Start up the application run loop
	return app.exec();
}
