#include "MainWindow.h"

MainWindowController::MainWindowController()
{
	m_mainWindow = new MainWindow(0);
	m_mainWindow->connectSignalsWithController(this);
}

MainWindowController::~MainWindowController()
{
}

void MainWindowController::showWidget()
{
	ilInit();
	m_mainWindow->show();
}

void MainWindowController::onEncodeButton()
{
	std::cout << "Encode Button Pushed" << std::endl;
	m_mainWindow->m_holoEncoder->encode(m_mainWindow->m_holoImage);
	
	ImageIO io;
	io.saveRGBImage("/Users/Karpinsn/Workspace/3DMVLab/HoloimageViewer/trunk/Holoimage.png", m_mainWindow->m_holoImage, 512, 512);
	
	//m_mainWindow->m_holoViewer->setHoloImage(m_mainWindow->m_holoImage);
}

MainWindow::MainWindow(QMainWindow* parent) : QMainWindow(parent)
{
	//	Create room for a 3 color holoimage
	m_holoImage = new unsigned char [3 * 512 * 512];
	memset(m_holoImage, 0, 512*512*3);
	
	// Sets up the interface elements from Designer file
	setupUi(this);
	/*
	// Setup the decoder widget
	glLayout->removeWidget(glDecoderView);
	glDecoderView->setParent(0);
	glDecoderView = NULL;
	
	m_holoViewer = new Holoviewer();
	m_viewerWidget = new OpenGLWidget(this, m_holoViewer, QColor::fromRgb(255, 255, 255, 0));
	m_viewerWidget->setMinimumSize(512, 512);
	m_viewerWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	glLayout->insertWidget(0, m_viewerWidget);*/
	
	// Setup the encoder widget
	glLayout->removeWidget(glView);
	glView->setParent(0);
	glView = NULL;
	
	m_holoEncoder = new Holoencoder();
	m_glWidget = new OpenGLWidget(this, m_holoEncoder, QColor::fromRgb(0, 0, 0, 0));
	m_glWidget->setMinimumSize(512, 512);
	m_glWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	glLayout->insertWidget(0, m_glWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectSignalsWithController(QObject* controller)
{
	// Connect the interface events (signals) to the controller class object
	connect(encodeButton, SIGNAL(clicked()), controller, SLOT(onEncodeButton()));
}
