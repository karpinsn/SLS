/**
 @file HolovideoCodecOptionsController.h
 */

#ifndef _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_
#define _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_HolovideoCodecOptions.h"

#include <QWidget>

#include "Codecs/HolovideoCodec.h"

class HolovideoCodecOptionsController : public QWidget, private Ui::HolovideoCodecOptions
{
  Q_OBJECT

private:
  HolovideoCodec* m_codec;

public:
  HolovideoCodecOptionsController(QWidget* parent = 0);
  ~HolovideoCodecOptionsController();

  Codec* getCodec(void);

public slots:
  void selectFile(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_
