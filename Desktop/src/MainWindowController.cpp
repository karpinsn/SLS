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
    m_mainWindow->encodeController->exportEntireVideo(m_mainWindow->fileList);
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
    m_mainWindow->viewController->cameraSelectMode(tool);

	if(m_mainWindow->captureController->isVisible())
	{
		m_mainWindow->captureController->cameraSelectMode(tool);
	}
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
