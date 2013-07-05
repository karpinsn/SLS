#ifndef _CAPTURE_TYPE_SELECT_DIALOG_H
#define _CAPTURE_TYPE_SELECT_DIALOG_H

#include "ICapture.h"
#include "ThreeFringeCapture.h"
#include "SixFringeCapture.h"
#include "NineFringeCapture.h"

//	Qt auto generated headers
#include "ui_CaptureTypeSelectDialog.h"

using namespace std;

class CaptureTypeSelectDialog : public QDialog, private Ui::CaptureTypeSelectDialog
{
    Q_OBJECT

public:
    CaptureTypeSelectDialog(QWidget *parent = 0);

	shared_ptr<ICapture> getCaptureType(void);
private:
  void _initCaptureTypeList(void);
};

#endif // _CAPTURE_TYPE_SELECT_DIALOG_H
