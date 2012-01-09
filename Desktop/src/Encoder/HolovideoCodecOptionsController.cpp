#include "HolovideoCodecOptionsController.h"

HolovideoCodecOptionsController::HolovideoCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = NULL;
  setupUi(this);

  _connectSignalsWithController();
}

HolovideoCodecOptionsController::~HolovideoCodecOptionsController()
{
  if(NULL != m_codec)
  {
	delete m_codec;
  }
}

void HolovideoCodecOptionsController::selectFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select source file to Open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

Codec* HolovideoCodecOptionsController::getCodec(void)
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

void HolovideoCodecOptionsController::_connectSignalsWithController(void)
{
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
}
