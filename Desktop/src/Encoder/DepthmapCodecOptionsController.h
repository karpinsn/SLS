/**
 @file DepthmapCodecOptionsController.h
 */

#ifndef _DEPTHMAP_CODEC_OPTIONS_CONTROLLER_H_
#define _DEPTHMAP_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_DepthmapCodecOptions.h"

#include <QWidget>

#include "Codecs/DepthCodec.h"

class DepthmapCodecOptionsController : public QWidget, private Ui::DepthmapCodecOptions
{
  Q_OBJECT

private:
  DepthCodec* m_codec;

public:
  DepthmapCodecOptionsController(QWidget* parent = 0);
  ~DepthmapCodecOptionsController();

  Codec* getCodec(void);


public slots:
  void contrastStretchValueChange(int checkState);
  void selectFile(void);

private:
  void _connectSignalsWithController(void);
};

#endif	// _DEPTHMAP_CODEC_OPTIONS_CONTROLLER_H_
