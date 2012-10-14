/**
 @file HoloimageEncodercOptionsController.h
 */

#ifndef _HOLOIMAGE_ENCODER_OPTIONS_CONTROLLER_H_
#define _HOLOIMAGE_ENCODER_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_HoloimageEncoderOptions.h"

#include <QWidget>

#include "Codecs/HoloimageEncoder.h"

class HoloimageEncoderOptionsController : public QWidget, private Ui::HoloimageEncoderOptions
{
  Q_OBJECT

private:
  shared_ptr<HoloimageEncoder> m_codec;

public:
  HoloimageEncoderOptionsController(QWidget* parent = 0);
  ~HoloimageEncoderOptionsController();

  shared_ptr<Codec> getCodec(void);

public slots:
  void selectFile(void);
  void autoFitData(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _HOLOIMAGE_ENCODER_OPTIONS_CONTROLLER_H_
