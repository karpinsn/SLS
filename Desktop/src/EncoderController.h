/*
 Filename:		EncoderController.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
 */

#ifndef _ENCODER_CONTROLLER_H_
#define _ENCODER_CONTROLLER_H_

//	Qt auto generated headers
#include "ui_Encode.h"

#include <QWidget>

#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "XYZFileIO.h"

#include "Codecs/DepthEncoder.h"
#include "Codecs/SixFringeDecoder.h"
#include "Codecs/NineFringeDecoder.h"
#include "Codecs/HoloimageEncoder.h"
#include "Codecs/HolovideoEncoder.h"
#include "Codecs/HolovideoDecoder.h"
#include "Codecs/XYZMDecoder.h"

#include "Holodecoder.h"

#include "VideoIO.h"

class EncoderController : public QWidget, private Ui::Encode
{
  Q_OBJECT

public:
  EncoderController(QWidget* parent = 0);
  ~EncoderController();

  void init(void);

public slots:
  void encode(void);
  void newEncoder(const QString& text);
  void newDecoder(const QString& text);

private:
  void _updateGL(void);
  void _connectSignalsWithController(void);
  void _addCodecs(void);
  void _previewEncoding(void);

  Codec* _getEncoder(void);
  Codec* _getDecoder(void);
};

#endif	// _ENCODER_CONTROLLER_H_
