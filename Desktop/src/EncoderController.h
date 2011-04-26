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

#include <QWidget>

#include "OpenGLWidget.h"
#include "Holoencoder.h"
#include "XYZFileIO.h"

class EncoderController : public QWidget
{
  Q_OBJECT

private:
  Holoencoder m_encoder;

public:
  EncoderController(QWidget* parent = 0);
  ~EncoderController();

  void exportCurrentFrame(void);
  void exportEntireVideo(void);
  void selectXYZM(const string &filename);

  void init(void);

private:
  void _updateGL(void);
};

#endif	// _ENCODER_CONTROLLER_H_
