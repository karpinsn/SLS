#include "HolovideoCodecOptionsController.h"

HolovideoCodecOptionsController::HolovideoCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  m_codec = NULL;
  setupUi(this);
}

HolovideoCodecOptionsController::~HolovideoCodecOptionsController()
{
  if(NULL != m_codec)
  {
	delete m_codec;
  }
}

Codec* HolovideoCodecOptionsController::getCodec(void)
{
  // Lazy init
  if(m_codec == NULL)
  {
	m_codec = new HolovideoCodec();
  }

  return m_codec;
}
