#include "SixFringeCodecOptionsController.h"

SixFringeCodecOptionsController::SixFringeCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);

  _connectSignalsWithController();
}

SixFringeCodecOptionsController::~SixFringeCodecOptionsController()
{
}

Codec* SixFringeCodecOptionsController::getCodec(void)
{
  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  SixFringeDecoder* codec = new SixFringeDecoder(str);
  codec->setGammaCutoff(gammaCutoffBox->value());
  codec->setScalingFactor(scalingBox->value());

  return codec;
}

void SixFringeCodecOptionsController::selectFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select file to open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void SixFringeCodecOptionsController::_connectSignalsWithController(void)
{
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
}