#include "NineFringeCodecOptionsController.h"

NineFringeCodecOptionsController::NineFringeCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);

  _connectSignalsWithController();
}

shared_ptr<Codec> NineFringeCodecOptionsController::getCodec(void)
{
  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  shared_ptr<NineFringeDecoder> codec = shared_ptr<NineFringeDecoder>(new NineFringeDecoder(str));
  codec->setGammaCutoff(gammaCutoffBox->value());
  codec->setScalingFactor(scalingBox->value());

  return codec;
}

void NineFringeCodecOptionsController::selectFile(void)
{
  QString file = QFileDialog::getSaveFileName(this, "Select file to open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void NineFringeCodecOptionsController::_connectSignalsWithController(void)
{
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
}