#include "DepthmapCodecOptionsController.h"

DepthmapCodecOptionsController::DepthmapCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
}

DepthmapCodecOptionsController::~DepthmapCodecOptionsController()
{
}

Codec* DepthmapCodecOptionsController::getDepthmapCodec(void)
{
  DepthCodec* codec = new DepthCodec();
   
  if(contrastStretchCheckBox->checkState() == Qt::Checked)
  {
	codec->enableContrastStretching(minValueBox.
  }

  return new DepthCodec();
}