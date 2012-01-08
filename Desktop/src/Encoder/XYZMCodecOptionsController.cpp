#include "XYZMCodecOptionsController.h"

XYZMCodecOptionsController::XYZMCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = NULL;

  setupUi(this);

  _connectSignalsWithController();
}

XYZMCodecOptionsController::~XYZMCodecOptionsController()
{
  if(NULL != m_codec)
  {
	delete m_codec;
  }
}

Codec* XYZMCodecOptionsController::getCodec(void)
{
  // Lazy init
  if(NULL == m_codec)
  {
	m_codec = new XYZMCodec(xyzmListWidget);
  }

  return m_codec;
}

void XYZMCodecOptionsController::addFiles(void)
{
  QStringList files = QFileDialog::getOpenFileNames(this, "Select XYZM Files to Open", "/", "Images (*.xyzm)");
	
	QStringList::const_iterator fileIterator;
	
	for (fileIterator = files.constBegin(); fileIterator != files.constEnd(); fileIterator++)
	{
		QListWidgetItem *xyzmFileWidget = new QListWidgetItem();
		xyzmFileWidget->setText(fileIterator->toLocal8Bit().constData());
		xyzmListWidget->addItem(xyzmFileWidget);
	}
	
	//	If we dont have a currently selected item then selected the first in the list
	if (NULL == xyzmListWidget->currentItem())
	{
		if(xyzmListWidget->count() > 0)
		{
			xyzmListWidget->setCurrentRow(0);
		}
	}
}

void XYZMCodecOptionsController::_connectSignalsWithController(void)
{
  //  Connect signals with controller
  connect(addButton, SIGNAL(clicked()), this, SLOT(addFiles()));
}