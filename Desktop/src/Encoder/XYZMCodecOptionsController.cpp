#include "XYZMCodecOptionsController.h"

XYZMCodecOptionsController::XYZMCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
}

XYZMCodecOptionsController::~XYZMCodecOptionsController()
{
}

Codec* XYZMCodecOptionsController::getCodec(void)
{
  XYZMCodec* codec = new XYZMCodec();

  return codec;
}
