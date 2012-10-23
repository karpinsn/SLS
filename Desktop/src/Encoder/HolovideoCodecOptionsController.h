/**
 @file HolovideoCodecOptionsController.h
 */

#ifndef _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_
#define _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_HolovideoCodecOptions.h"

#include <QWidget>

#include "Codecs/HolovideoEncoder.h"

class HolovideoCodecOptionsController : public QWidget, private Ui::HolovideoCodecOptions
{
  Q_OBJECT

private:
  shared_ptr<HolovideoEncoder> m_codec;

public:
  HolovideoCodecOptionsController(QWidget* parent = 0);

  shared_ptr<Codec> getCodec(void);

public slots:
  void selectFile(void);
  void autoFitData(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _HOLOVIDEO_CODEC_OPTIONS_CONTROLLER_H_
