#include "MultiWavelengthCodecOptionsController.h"

MultiWavelengthCodecOptionsController::MultiWavelengthCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
}

MultiWavelengthCodecOptionsController::~MultiWavelengthCodecOptionsController()
{
}

Codec* MultiWavelengthCodecOptionsController::getCodec(void)
{
  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  MultiWavelengthCodec* codec = new MultiWavelengthCodec(str);
  codec->setGammaCutoff(gammaCutoffBox->value());
  codec->setScalingFactor(scalingBox->value());

  return codec;
}
