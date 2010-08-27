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
	m_mainWindow->show();
}

void MainWindowController::onEncodeButton()
{
	std::cout << "Encode Button Pushed" << std::endl;
	//GLuint texID = m_mainWindow->m_holoEncoder->encode(m_mainWindow->m_holoImage);
	
	ImageIO io;
	//io.saveRGBImage("/Users/Karpinsn/Holoimage.png", 512, 512);
	io.saveAviFile("C:/Users/Nikolaus Karpinsky/Data/HoloVideo.avi", 512, 512, 30);
	
	XYZFileIO fileIO;
	
	for(int itemNumber = 0; itemNumber < m_mainWindow->fileList->count(); itemNumber++)
	{
		QListWidgetItem *item = m_mainWindow->fileList->item(itemNumber);
	
		AbstractMesh* currentMesh = fileIO.newMeshFromFile(item->text().toStdString());
		m_mainWindow->m_holoEncoder->setCurrentMesh(currentMesh);
		
		GLuint texID = m_mainWindow->m_holoEncoder->encode();
		io.saveAviFileWriteFrame(texID, 512, 512);
	}
	
	clog << "Encoding complete" << endl;
	io.saveAviFileFinish();
	
	//m_mainWindow->m_holoViewer->setHoloImage(m_mainWindow->m_holoImage);
}

void MainWindowController::onOpenXYZM()
{
	QStringList files = QFileDialog::getOpenFileNames(m_mainWindow, "Select XYZM Files to Open", "/home", "Images (*.xyzm)");
	
	QStringList::const_iterator fileIterator;
	
	for (fileIterator = files.constBegin(); fileIterator != files.constEnd(); fileIterator++)
	{
		QListWidgetItem *xyzmFileWidget = new QListWidgetItem();
		xyzmFileWidget->setText(fileIterator->toLocal8Bit().constData());
		m_mainWindow->fileList->addItem(xyzmFileWidget);
	}
	
	//	If we dont have a currently selected item then selected the first in the list
	if (NULL == m_mainWindow->fileList->currentItem())
	{
		m_mainWindow->fileList->setCurrentRow(0);
	}
}

void MainWindowController::selectXYZM(QListWidgetItem* current, QListWidgetItem* previous)
{
	//	Display the new XYZM file
	cout << "New file selected" << endl;
	
	XYZFileIO fileIO;
	AbstractMesh* currentMesh = fileIO.newMeshFromFile(current->text().toStdString());
	m_mainWindow->m_holoEncoder->setCurrentMesh(currentMesh);
	m_mainWindow->m_glWidget->updateScene();
}

void MainWindowController::playVideo(void)
{
	m_mainWindow->m_glWidget->setNewGLContext(m_mainWindow->m_holoDecoder);
	m_mainWindow->m_glWidget->playMovie(m_mainWindow->m_holoDecoder);
	
	//m_videoThread = new HolovideoThread(m_mainWindow->m_holoDecoder, m_mainWindow->m_glWidget);
	//m_videoThread->start();
}

MainWindow::MainWindow(QMainWindow* parent) : QMainWindow(parent)
{
	//	Create room for a 3 color holoimage
	m_holoImage = new unsigned char [3 * 512 * 512];
	memset(m_holoImage, 0, 512*512*3);
	
	// Sets up the interface elements from Designer file
	setupUi(this);
	
	// Setup the encoder widget
	horizontalLayout_3->removeWidget(glView);
	glView->setParent(0);
	glView = NULL;
	
	m_holoEncoder = new Holoencoder();
	m_glWidget = new OpenGLWidget(this, m_holoEncoder, QColor::fromRgb(0, 0, 0, 0));
	m_glWidget->setMinimumSize(512, 512);
	m_glWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	horizontalLayout_3->insertWidget(0, m_glWidget);
	m_holoDecoder = new Holodecoder(m_glWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectSignalsWithController(QObject* controller)
{
	// Connect the interface events (signals) to the controller class object
	connect(encodeButton, SIGNAL(clicked()), controller, SLOT(onEncodeButton()));
	connect(fileList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), controller, SLOT(selectXYZM(QListWidgetItem*, QListWidgetItem*)));
	connect(actionOpenXYZM, SIGNAL(triggered()), controller, SLOT(onOpenXYZM()));
	connect(actionOpen_Holovideo, SIGNAL(triggered()), controller, SLOT(playVideo()));
}
