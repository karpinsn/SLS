/**
 @file MultiWavelengthCodecOptionsController.h
 */

#ifndef _MULTI_WAVELENGTH_CODEC_OPTIONS_CONTROLLER_H_
#define _MULTI_WAVELENGTH_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_MultiWavelengthCodecOptions.h"

#include <QWidget>

#include "Codecs/MultiWavelengthCodec.h"

class MultiWavelengthCodecOptionsController : public QWidget, private Ui::MultiWavelengthCodecOptions
{
  Q_OBJECT

public:
  MultiWavelengthCodecOptionsController(QWidget* parent = 0);
  ~MultiWavelengthCodecOptionsController();

  Codec* getCodec(void);
};

#endif	// _MULTI_WAVELENGTH_CODEC_OPTIONS_CONTROLLER_H_
