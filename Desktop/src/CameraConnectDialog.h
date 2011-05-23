#ifndef CAMERA_CONNECT_DIALOG_H
#define CAMERA_CONNECT_DIALOG_H

#include "lens/Camera.h"
#include "lens/OpenCVCamera.h"
#include "lens/JAICamera.h"

//	Qt auto generated headers
#include "ui_CameraConnectDialog.h"

class CameraConnectDialog : public QDialog, private Ui::CameraConnectDialog
{
    Q_OBJECT

public:
    CameraConnectDialog(QWidget *parent = 0);

    lens::Camera* getCamera(void);

private:
    void _initCameraDriverList(void);
};

#endif // CAMERA_CONNECT_DIALOG_H
