#include "HolovideoCodecOptionsController.h"

HolovideoCodecOptionsController::HolovideoCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = nullptr;
  setupUi(this);

  _connectSignalsWithController();
}

HolovideoCodecOptionsController::~HolovideoCodecOptionsController()
{
}

void HolovideoCodecOptionsController::selectFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select source file to Open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void HolovideoCodecOptionsController::autoFitData(void)
{
  if(nullptr != m_codec)
  {
	m_codec->autoFitData();
  }
}

shared_ptr<Codec> HolovideoCodecOptionsController::getCodec(void)
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

void HolovideoCodecOptionsController::_connectSignalsWithController(void)
{
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
  connect(autoFitDataButton, SIGNAL(clicked()), this, SLOT(autoFitData()));
}
