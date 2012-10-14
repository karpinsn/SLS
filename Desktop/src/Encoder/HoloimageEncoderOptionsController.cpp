#include "HoloimageEncoderOptionsController.h"

HoloimageEncoderOptionsController::HoloimageEncoderOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = nullptr;
  setupUi(this);

  _connectSignalsWithController();
}

HoloimageEncoderOptionsController::~HoloimageEncoderOptionsController()
{
}

void HoloimageEncoderOptionsController::selectFile(void)
{
  QString file = QFileDialog::getSaveFileName(this, "Save File As", "/", "Images (*.png)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void HoloimageEncoderOptionsController::autoFitData(void)
{
  if(nullptr != m_codec)
  {
	m_codec->autoFitData();
  }
}

shared_ptr<Codec> HoloimageEncoderOptionsController::getCodec(void)
{
  // Lazy init
  if(m_codec == nullptr)
  {
    m_codec = shared_ptr<HoloimageEncoder>(new HoloimageEncoder());
  }

  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  m_codec->initCodec(str, widthBox->value(), heightBox->value());

  return m_codec;
}

void HoloimageEncoderOptionsController::_connectSignalsWithController(void)
{
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
  connect(autoFitDataButton, SIGNAL(clicked()), this, SLOT(autoFitData()));
}
