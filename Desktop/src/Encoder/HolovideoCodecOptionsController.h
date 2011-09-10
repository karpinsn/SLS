/**
 @file HolovideoCodecOptionsController.h
 */

#ifndef _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_
#define _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_DepthmapCodecOptions.h"

#include <QWidget>

#include "Codecs/HolovideoCodec.h"

class HolovideoCodecOptionsController : public QWidget, private Ui::HolovideoCodecOptions
{
  Q_OBJECT

public:
  HolovideoCodecOptionsController(QWidget* parent = 0);
  ~HolovideoCodecOptionsController();

  Codec* getCodec(void);
};

#endif	// _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_
