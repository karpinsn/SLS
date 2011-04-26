    #include "MainWindowController.h"

MainWindowController::MainWindowController()
{
	m_mainWindow = new MainWindowView(0);
	m_mainWindow->connectSignalsWithController(this);
}

MainWindowController::~MainWindowController()
{
}

void MainWindowController::showWidget()
{
  m_mainWindow->mainView->setCurrentWidget(m_mainWindow->viewController);
  m_mainWindow->show();
}

void MainWindowController::exportSingleFrame()
{
	Logger::logDebug("MainWindowController - exportSingleFrame: Enter");
    m_mainWindow->encodeController->exportCurrentFrame();
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
    m_mainWindow->encodeController->selectXYZM(current->text().toAscii().constData());
}

void MainWindowController::playVideo(void)
{
	QString file = QFileDialog::getOpenFileName(m_mainWindow, "Select Holovideo to Open", "/", "Images (*.avi *.mp4 *.mov *.mkv)");
		
	if(!file.isEmpty())
	{
      m_mainWindow->viewController->playMovie(file.toStdString());
	}
}

void MainWindowController::openHoloImage(void)
{
  m_mainWindow->viewController->openHoloImage();
}

void MainWindowController::toolSelect(int tool)
{
    //m_mainWindow->glWidget->cameraSelectMode(tool);
}

void MainWindowController::viewMode(void)
{
  //  Ensure that the file list is hidden
  m_mainWindow->mainView->setCurrentWidget(m_mainWindow->viewController);
}

void MainWindowController::encodeMode(void)
{
  //  Show the file list
  //m_mainWindow->showFileList();

  m_mainWindow->mainView->setCurrentWidget(m_mainWindow->encodeController);
}

void MainWindowController::captureMode(void)
{
  m_mainWindow->mainView->setCurrentWidget(m_mainWindow->captureController);

  /*
    //  Ensure that the file list is hidden
    //m_mainWindow->hideFileList();
    MultiWavelengthCapture *capture = new MultiWavelengthCapture();
    //m_mainWindow->glWidget->setNewGLContext(capture);
    capture->loadTestData();
    //m_mainWindow->glWidget->updateGL();
    capture->loadTestData();
    //m_mainWindow->glWidget->updateGL();
    */
}
