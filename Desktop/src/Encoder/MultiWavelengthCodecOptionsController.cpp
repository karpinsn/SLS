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
  MultiWavelengthCodec* codec = new MultiWavelengthCodec();

  codec->setGammaCutoff(gammaCutoffBox->value());
  codec->setScalingFactor(scalingBox->value());

  return codec;
}
