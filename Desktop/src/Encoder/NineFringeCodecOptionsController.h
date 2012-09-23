/**
 @file NineFringeCodecOptionsController.h
 */

#ifndef _NINE_FRINGE_CODEC_OPTIONS_CONTROLLER_H_
#define _NINE_FRINGE_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_NineFringeCodecOptions.h"

#include <QWidget>

#include "Codecs/NineFringeDecoder.h"

class NineFringeCodecOptionsController : public QWidget, private Ui::NineFringeCodecOptions
{
  Q_OBJECT

public:
  NineFringeCodecOptionsController(QWidget* parent = 0);
  ~NineFringeCodecOptionsController();

  Codec* getCodec(void);

public slots:
  void selectFile(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _NINE_FRINGE_CODEC_OPTIONS_CONTROLLER_H_
