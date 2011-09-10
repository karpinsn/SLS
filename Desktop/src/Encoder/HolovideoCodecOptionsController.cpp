#include "HolovideoCodecOptionsController.h"

HolovideoCodecOptionsController::HolovideoCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
}

HolovideoCodecOptionsController::~HolovideoCodecOptionsController()
{
}

Codec* HolovideoCodecOptionsController::getCodec(void)
{
  HolovideoCodec* codec = new HolovideoCodec();

  return codec;
}
