#include "CameraConnectDialog.h"

CameraConnectDialog::CameraConnectDialog(QWidget *parent) : QDialog(parent)
{
    // Setup dialog
    setupUi(this);

    setWindowModality(Qt::ApplicationModal);
}

lens::Camera* CameraConnectDialog::getCamera(void)
{
  lens::Camera *m_camera = NULL;

  if(this->exec() == QDialog::Accepted)
  {
    //  Check if the openCV camera is selected
    m_camera = new lens::OpenCVCamera();
  }

  return m_camera;
}
