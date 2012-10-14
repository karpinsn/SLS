#include "DepthmapCodecOptionsController.h"

DepthmapCodecOptionsController::DepthmapCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = nullptr;

  setupUi(this);
  _connectSignalsWithController();
}

DepthmapCodecOptionsController::~DepthmapCodecOptionsController()
{
}

void DepthmapCodecOptionsController::contrastStretchValueChange(int checkState)
{
  if(Qt::Checked == checkState)
  {
    minValueBox->setReadOnly(false);
    maxValueBox->setReadOnly(false);
  }
  else
  {
    minValueBox->setReadOnly(true);
    maxValueBox->setReadOnly(true);
  }
}

shared_ptr<Codec> DepthmapCodecOptionsController::getCodec(void)
{
  // Lazy init
  if(nullptr == m_codec)
  {
    m_codec = shared_ptr<DepthEncoder>(new DepthEncoder());
  }
   
  if(Qt::Checked == contrastStretchCheckBox->checkState())
  {
    m_codec->enableContrastStretching(minValueBox->value(), maxValueBox->value());
  }
  else
  {
    m_codec->disableContrastStretching();
  }

  m_codec->setFilename(sourceFileBox->text().toAscii().constData());

  return m_codec;
}

void DepthmapCodecOptionsController::selectFile(void)
{
  QString file = QFileDialog::getSaveFileName(this, "Save File as", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void DepthmapCodecOptionsController::_connectSignalsWithController(void)
{
  connect(contrastStretchCheckBox, SIGNAL(stateChanged(int)), this, SLOT(contrastStretchValueChange(int)));
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
}
