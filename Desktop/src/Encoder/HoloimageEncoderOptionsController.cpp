#include "HoloimageEncoderOptionsController.h"

HoloimageEncoderOptionsController::HoloimageEncoderOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = NULL;
  setupUi(this);

  _connectSignalsWithController();
}

HoloimageEncoderOptionsController::~HoloimageEncoderOptionsController()
{
  if(NULL != m_codec)
  {
	delete m_codec;
  }
}

void HoloimageEncoderOptionsController::selectFile(void)
{
  QString file = QFileDialog::getSaveFileName(this, "Select source file to Open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void HoloimageEncoderOptionsController::autoFitData(void)
{
  if(NULL != m_codec)
  {
	m_codec->autoFitData();
  }
}

Codec* HoloimageEncoderOptionsController::getCodec(void)
{
  // Lazy init
  if(m_codec == NULL)
  {
    m_codec = new HoloimageEncoder();
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
