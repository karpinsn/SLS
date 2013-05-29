#include "CameraConnectDialog.h"

CameraConnectDialog::CameraConnectDialog(QWidget *parent) : QDialog(parent)
{
  // Setup dialog
  setupUi(this);
  _initCameraDriverList();

  setWindowModality(Qt::ApplicationModal);
}

lens::ICamera* CameraConnectDialog::getCamera(void)
{
  lens::ICamera *m_camera = nullptr;

  if(this->exec() == QDialog::Accepted)
  {
    if(0 == QString(lens::OpenCVCamera::cameraName().c_str()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::OpenCVCamera();
    }
    else if(0 == QString(lens::FileCamera::cameraName().c_str()).compare(cameraDriverComboBox->currentText()))
    {
	  //TODO: Comeback and fix
      //m_camera = new lens::FileCamera();
    }

#ifdef USE_IC_CAMERA
    if(0 == QString(lens::ICCamera::cameraName().c_str()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::ICCamera();
    }
#endif

#ifdef USE_JAI_CAMERA
    if(0 == QString(lens::JAICamera::cameraName().c_str()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::JAICamera();
    }
#endif

#ifdef USE_POINT_GREY_CAMERA
	if(0 == QString(lens::PointGreyCamera::cameraName().c_str()).compare(cameraDriverComboBox->currentText()))
	{
	  m_camera = new lens::PointGreyCamera();
	}
#endif

#ifdef USE_PHANTOM_CAMERA
	if(0 == QString(lens::PhantomCamera::cameraName().c_str()).compare(cameraDriverComboBox->currentText()))
    {
      m_camera = new lens::PhantomCamera();
    }
#endif
  }

  return m_camera;
}

void CameraConnectDialog::_initCameraDriverList(void)
{
	cameraDriverComboBox->addItem(QString(lens::OpenCVCamera::cameraName().c_str()));
  cameraDriverComboBox->addItem(QString(lens::FileCamera::cameraName().c_str()));

  //  Only add the cameras if they are being used
#ifdef USE_IC_CAMERA
  cameraDriverComboBox->addItem(QString(lens::ICCamera::cameraName().c_str()));
#endif

#ifdef USE_JAI_CAMERA
  cameraDriverComboBox->addItem(QString(lens::JAICamera::cameraName().c_str()));
#endif

#ifdef USE_POINT_GREY_CAMERA
	cameraDriverComboBox->addItem(QString(lens::PointGreyCamera::cameraName().c_str()));
#endif

#ifdef USE_PHANTOM_CAMERA
  cameraDriverComboBox->addItem(QString(lens::PhantomCamera::cameraName().c_str()));
#endif

}
