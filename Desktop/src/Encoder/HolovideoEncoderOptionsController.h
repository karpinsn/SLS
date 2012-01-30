/**
 @file HolovideoEncodercOptionsController.h
 */

#ifndef _HOLOVIDEO_ENCODER_OPTIONS_CONTROLLER_H_
#define _HOLOVIDEO_ENCODER_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_HolovideoCodecOptions.h"

#include <QWidget>

#include "Codecs/HolovideoEncoder.h"

class HolovideoEncoderOptionsController : public QWidget, private Ui::HolovideoCodecOptions
{
  Q_OBJECT

private:
  HolovideoEncoder* m_codec;

public:
  HolovideoEncoderOptionsController(QWidget* parent = 0);
  ~HolovideoEncoderOptionsController();

  Codec* getCodec(void);

public slots:
  void selectFile(void);
  void autoFitData(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _HOLOVIDEO_ENCODER_OPTIONS_CONTROLLER_H_
