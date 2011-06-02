#include "CameraConnectDialog.h"

CameraConnectDialog::CameraConnectDialog(QWidget *parent) : QDialog(parent)
{
  // Setup dialog
  setupUi(this);
  _initCameraDriverList();

  setWindowModality(Qt::ApplicationModal);
}

lens::Camera* CameraConnectDialog::getCamera(void)
{
  lens::Camera *m_camera = NULL;

  if(this->exec() == QDialog::Accepted)
  {
    if(0 == QString::fromStdString(lens::OpenCVCamera::cameraName()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::OpenCVCamera();
    }
    else if(0 == QString::fromStdString(lens::FileCamera::cameraName()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::FileCamera();
    }

#ifdef USE_IC_CAMERA
    if(0 == QString::fromStdString(lens::ICCamera::cameraName()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::ICCamera();
    }
#endif

#ifdef USE_JAI_CAMERA
    if(0 == QString::fromStdString(lens::JAICamera::cameraName()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::JAICamera();
    }
#endif
  }

  return m_camera;
}

void CameraConnectDialog::_initCameraDriverList(void)
{
  cameraDriverComboBox->addItem(QString::fromStdString(lens::OpenCVCamera::cameraName()));
  cameraDriverComboBox->addItem(QString::fromStdString(lens::FileCamera::cameraName()));

  //  Only add the cameras if they are being used
#ifdef USE_IC_CAMERA
  cameraDriverComboBox->addItem(QString::fromStdString(lens::ICCamera::cameraName()));
#endif

#ifdef USE_JAI_CAMERA
  cameraDriverComboBox->addItem(QString::fromStdString(lens::JAICamera::cameraName()));
#endif

}
