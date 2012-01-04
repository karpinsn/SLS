/**
 @file HolovideoCodecOptionsController.h
 */

#ifndef _XYZM_CODEC_OPTIONS_CONTROLLER_H_
#define _XYZM_CODEC_OPTIONS_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_XYZMCodecOptions.h"

#include "Codecs/XYZMCodec.h"
#include <QWidget>

class XYZMCodecOptionsController : public QWidget, private Ui::XYZMCodecOptions
{
  Q_OBJECT

private:
  XYZMCodec* m_codec;

public:
  XYZMCodecOptionsController(QWidget* parent = 0);
  ~XYZMCodecOptionsController();

  Codec* getCodec(void);

  public slots:
	void addFiles(void);
};

#endif	// _XYZM_CODEC_OPTIONS_CONTROLLER_H_
