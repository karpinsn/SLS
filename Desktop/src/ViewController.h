/*
 Filename:		CameraController.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
 */

#ifndef _VIEW_CONTROLLER_H_
#define _VIEW_CONTROLLER_H_

#include <QWidget>
#include <QtCore/QTimer>

#include "OpenGLWidget.h"

class ViewController : public QWidget
{
  Q_OBJECT

private:
  Holodecoder m_decoder;
  QTime m_movieTimer;
  ImageIO m_aviIO;

public:
  ViewController(QWidget* parent = 0);
  ~ViewController();


  void init(void);
  void cameraSelectMode(int mode);
  void playMovie(string movieFile);
  void openHoloImage(void);

protected:
  void timerEvent(QTimerEvent* event);

private:
  void _updateGL(void);

};

#endif	// _VIEW_CONTROLLER_H_
