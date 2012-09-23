#include "NineFringeCodecOptionsController.h"

NineFringeCodecOptionsController::NineFringeCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
}

NineFringeCodecOptionsController::~NineFringeCodecOptionsController()
{
}

Codec* NineFringeCodecOptionsController::getCodec(void)
{
  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  NineFringeDecoder* codec = new NineFringeDecoder(str);
  codec->setGammaCutoff(gammaCutoffBox->value());
  codec->setScalingFactor(scalingBox->value());

  return codec;
}
