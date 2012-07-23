#include "CaptureTypeSelectDialog.h"

CaptureTypeSelectDialog::CaptureTypeSelectDialog(QWidget *parent) : QDialog(parent)
{
  // Setup dialog
  setupUi(this);

  _initCaptureTypeList();
  setWindowModality(Qt::ApplicationModal);
}

shared_ptr<ICapture> CaptureTypeSelectDialog::getCaptureType(void)
{
  shared_ptr<ICapture> captureType = nullptr;

  if(this->exec() == QDialog::Accepted)
  {
	if(0 == QString(ThreeFringeCapture::getCaptureName().c_str()).compare(captureTypeComboBox->currentText()))
	{
	  captureType = make_shared<ThreeFringeCapture>();
	}
	else if(0 == QString(NineFringeCapture::getCaptureName().c_str()).compare(captureTypeComboBox->currentText()))
	{
	  captureType = make_shared<NineFringeCapture>();
	}
  }

  return captureType;
}

void CaptureTypeSelectDialog::_initCaptureTypeList(void)
{
  captureTypeComboBox->addItem(QString(ThreeFringeCapture::getCaptureName().c_str()));
  captureTypeComboBox->addItem(QString(NineFringeCapture::getCaptureName().c_str()));
}