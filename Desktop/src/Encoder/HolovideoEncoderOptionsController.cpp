#include "HolovideoEncoderOptionsController.h"

HolovideoEncoderOptionsController::HolovideoEncoderOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = NULL;
  setupUi(this);

  _connectSignalsWithController();
}

HolovideoEncoderOptionsController::~HolovideoEncoderOptionsController()
{
  if(NULL != m_codec)
  {
	delete m_codec;
  }
}

void HolovideoEncoderOptionsController::selectFile(void)
{
  QString file = QFileDialog::getSaveFileName(this, "Save File As", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void HolovideoEncoderOptionsController::autoFitData(void)
{
  if(NULL != m_codec)
  {
	m_codec->autoFitData();
  }
}

Codec* HolovideoEncoderOptionsController::getCodec(void)
{
  // Lazy init
  if(m_codec == NULL)
  {
    m_codec = new HolovideoEncoder();
  }

  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  m_codec->initCodec(str, widthBox->value(), heightBox->value());

  return m_codec;
}

void HolovideoEncoderOptionsController::_connectSignalsWithController(void)
{
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
  connect(autoFitDataButton, SIGNAL(clicked()), this, SLOT(autoFitData()));
}
