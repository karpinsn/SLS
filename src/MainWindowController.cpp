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
	m_mainWindow->show();
}

void MainWindowController::onEncodeButton()
{
	std::cout << "Encode Button Pushed" << std::endl;
	
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
			
			for(int itemNumber = 0; itemNumber < m_mainWindow->fileList->count(); itemNumber++)
			{
				//	Increase the progress
				progress.setValue(itemNumber);
				
				QListWidgetItem *item = m_mainWindow->fileList->item(itemNumber);
				
				AbstractMesh* currentMesh = fileIO.newMeshFromFile(item->text().toAscii().constData());
				m_mainWindow->m_holoEncoder->setCurrentMesh(currentMesh);
				
				GLuint texID = m_mainWindow->m_holoEncoder->encode();
				io.saveAviFileWriteFrame(texID, 512, 512);
			}
			
			//	Last one done!
			progress.setValue(m_mainWindow->fileList->count());
			
			clog << "Encoding complete" << endl;
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
	//	Display the new XYZM file
	cout << "New file selected" << endl;
	
	XYZFileIO fileIO;
	AbstractMesh* currentMesh = fileIO.newMeshFromFile(current->text().toAscii().constData());
	m_mainWindow->m_holoEncoder->setCurrentMesh(currentMesh);
	m_mainWindow->m_glWidget->updateScene();
}

void MainWindowController::playVideo(void)
{
	QString file = QFileDialog::getOpenFileName(m_mainWindow, "Select Holovideo to Open", "/", "Images (*.avi)");
		
	if(!file.isEmpty())
	{
		m_mainWindow->m_glWidget->setNewGLContext(m_mainWindow->m_holoDecoder);
		m_mainWindow->m_glWidget->playMovie(file.toStdString(), m_mainWindow->m_holoDecoder);
	}
}

void MainWindowController::openHoloImage(void)
{
	QString file = QFileDialog::getOpenFileName(m_mainWindow, "Select Holoimage to Open", "/", "Images (*.jpg)");
	
	if(!file.isEmpty())
	{
		m_mainWindow->m_glWidget->setNewGLContext(m_mainWindow->m_holoDecoder);
		m_mainWindow->m_glWidget->playMovie(file.toStdString(), m_mainWindow->m_holoDecoder);
	}
}

void MainWindowController::toolSelect(const int tool)
{
	m_mainWindow->m_glWidget->cameraSelectMode(tool);
}