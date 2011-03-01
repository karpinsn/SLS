#include "MainWindowController.h"

MainWindowController::MainWindowController()
{
	m_mainWindow = new MainWindowView(0);
	m_mainWindow->connectSignalsWithController(this);

        //  Since we start in view give it the Holodecoder
        Holodecoder *decoder = new Holodecoder();
        m_mainWindow->glWidget->m_holoDecoder = decoder;
        m_mainWindow->glWidget->m_glContext = decoder;
}

MainWindowController::~MainWindowController()
{
}

void MainWindowController::showWidget()
{
	m_mainWindow->show();
}

void MainWindowController::exportSingleFrame()
{
	Logger::logDebug("MainWindowController - exportSingleFrame: Enter");
	
	QString fileName = QFileDialog::getSaveFileName(m_mainWindow, "Save File", "/", "Images (*.png)");
	
	if(!fileName.isEmpty())
	{
		ImageIO io;
		Texture holoimage = m_mainWindow->m_holoEncoder->encode();
		io.saveTexture(fileName.toAscii().constData(), holoimage);
	}
}

void MainWindowController::exportEntireVideo()
{
	Logger::logDebug("MainWindowController - exportEntireVideo: Enter");
	
	QString fileName = QFileDialog::getSaveFileName(m_mainWindow, "Save File", "/", "Video (*.avi)");
	
	if(!fileName.isEmpty())
	{
		ImageIO io;
		bool canSaveFile = io.saveAviFile(fileName.toAscii().constData(), 512, 512, 30);
		
		if(canSaveFile)
		{
			XYZFileIO fileIO;
			
			//	Inform the user of the progress
			QProgressDialog progress("Encoding frames...", 0, 0, m_mainWindow->fileList->count(), m_mainWindow);
			
			Logger::logDebug("MainWindowController - exportEntireVideo: Encoding frames");
			for(int itemNumber = 0; itemNumber < m_mainWindow->fileList->count(); itemNumber++)
			{
				//	Increase the progress
				progress.setValue(itemNumber);
				
				QListWidgetItem *item = m_mainWindow->fileList->item(itemNumber);
				
				AbstractMesh* currentMesh = fileIO.newMeshFromFile(item->text().toAscii().constData());
				m_mainWindow->m_holoEncoder->setCurrentMesh(currentMesh);
				
				Texture holoimage = m_mainWindow->m_holoEncoder->encode();
				io.saveAviFileWriteFrame(holoimage);
			}
			
			//	Last one done!
			progress.setValue(m_mainWindow->fileList->count());
			
			Logger::logDebug("MainWindowController - exportEntireVideo: Encoding complete!");
			io.saveAviFileFinish();
		}
	}
}

void MainWindowController::onOpenXYZM()
{
	QStringList files = QFileDialog::getOpenFileNames(m_mainWindow, "Select XYZM Files to Open", "/", "Images (*.xyzm)");
	
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
		if(m_mainWindow->fileList->count() > 0)
		{
			m_mainWindow->fileList->setCurrentRow(0);
		}
	}
}

void MainWindowController::selectXYZM(QListWidgetItem* current, QListWidgetItem* previous)
{
	Logger::logDebug("MainWindowController - selectXYZM: Enter");

	//	Display the new XYZM file	
	XYZFileIO fileIO;
	AbstractMesh* currentMesh = fileIO.newMeshFromFile(current->text().toAscii().constData());
	m_mainWindow->m_holoEncoder->setCurrentMesh(currentMesh);
        m_mainWindow->glWidget->updateScene();
}

void MainWindowController::playVideo(void)
{
	QString file = QFileDialog::getOpenFileName(m_mainWindow, "Select Holovideo to Open", "/", "Images (*.avi *.mp4 *.mov *.mkv)");
		
	if(!file.isEmpty())
	{
                //m_mainWindow->glWidget->setNewGLContext(m_mainWindow->m_holoDecoder);
                m_mainWindow->glWidget->playMovie(file.toStdString());
	}
}

void MainWindowController::openHoloImage(void)
{
	QString file = QFileDialog::getOpenFileName(m_mainWindow, "Select Holoimage to Open", "/", "Images (*.png *.jpg)");
	
	if(!file.isEmpty())
	{
                //m_mainWindow->glWidget->setNewGLContext(m_mainWindow->m_holoDecoder);
                m_mainWindow->glWidget->openHoloImage(file.toStdString());
	}
}

void MainWindowController::toolSelect(int tool)
{
    m_mainWindow->glWidget->cameraSelectMode(tool);
}

void MainWindowController::viewMode(void)
{
    //  Ensure that the file list is hidden
    m_mainWindow->hideFileList();
    Holodecoder *decoder = new Holodecoder();
    m_mainWindow->glWidget->m_holoDecoder = decoder;
    m_mainWindow->glWidget->setNewGLContext(decoder);
}

void MainWindowController::encodeMode(void)
{
    //  Show the file list
    m_mainWindow->showFileList();
    Holoencoder *encoder = new Holoencoder();
    m_mainWindow->m_holoEncoder = encoder;
    m_mainWindow->glWidget->setNewGLContext(encoder);
}

void MainWindowController::captureMode(void)
{
    //  Ensure that the file list is hidden
    m_mainWindow->hideFileList();
    MultiWavelengthCapture *capture = new MultiWavelengthCapture();
    m_mainWindow->glWidget->setNewGLContext(capture);
    capture->loadTestData();
    m_mainWindow->glWidget->updateGL();
    capture->loadTestData();
    m_mainWindow->glWidget->updateGL();

}
