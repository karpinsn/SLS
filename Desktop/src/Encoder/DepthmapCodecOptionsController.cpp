#include "DepthmapCodecOptionsController.h"

DepthmapCodecOptionsController::DepthmapCodecOptionsController(QWidget* parent) : QWidget(parent)
{
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

Codec* DepthmapCodecOptionsController::getCodec(void)
{
  DepthCodec* codec = new DepthCodec();
   
  if(Qt::Checked == contrastStretchCheckBox->checkState())
  {
    codec->enableContrastStretching(minValueBox->value(), maxValueBox->value());
  }
  else
  {
    codec->disableContrastStretching();
  }

  return codec;
}

void DepthmapCodecOptionsController::_connectSignalsWithController(void)
{
  connect(contrastStretchCheckBox, SIGNAL(stateChanged(int)), this, SLOT(contrastStretchValueChange(int)));
}
