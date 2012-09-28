/**
 @file SixFringeCodecOptionsController.h
 */

#ifndef _SIX_FRINGE_CODEC_OPTIONS_CONTROLLER_H_
#define _SIX_FRINGE_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_SixFringeCodecOptions.h"

#include <QWidget>

#include "Codecs/SixFringeDecoder.h"

class SixFringeCodecOptionsController : public QWidget, private Ui::SixFringeCodecOptions
{
  Q_OBJECT

public:
  SixFringeCodecOptionsController(QWidget* parent = 0);
  ~SixFringeCodecOptionsController();

  Codec* getCodec(void);

public slots:
  void loadReferenceValueChange(int checkState);
  void selectShortWavelengthFile(void);
  void selectLongWavelengthFile(void);
  void selectFile(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _SIX_FRINGE_CODEC_OPTIONS_CONTROLLER_H_
