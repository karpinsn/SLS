#include "HolovideoEncoderOptionsController.h"

HolovideoEncoderOptionsController::HolovideoEncoderOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = nullptr;
  setupUi(this);

  _connectSignalsWithController();
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
  if(nullptr != m_codec)
  {
	m_codec->autoFitData();
  }
}

shared_ptr<Codec> HolovideoEncoderOptionsController::getCodec(void)
{
  // Lazy init
  if(m_codec == nullptr)
  {
    m_codec = shared_ptr<HolovideoEncoder>(new HolovideoEncoder());
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
