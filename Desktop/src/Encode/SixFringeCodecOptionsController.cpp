#include "SixFringeCodecOptionsController.h"

SixFringeCodecOptionsController::SixFringeCodecOptionsController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);

  _connectSignalsWithController();
}

shared_ptr<Codec> SixFringeCodecOptionsController::getCodec(void)
{
  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toLocal8Bit().constData();

  shared_ptr<SixFringeDecoder> codec = shared_ptr<SixFringeDecoder>(new SixFringeDecoder(str));
  codec->setGammaCutoff(gammaCutoffBox->value());
  codec->setScalingFactor(scalingBox->value());

  if(loadReferenceCheckbox->isChecked() && !shortReferenceFileBox->text().isEmpty() && !longReferenceFileBox->text().isEmpty())
  {
	ImageIO io;
	shared_ptr<IplImage> shortImage(io.readImage(shortReferenceFileBox->text().toAscii().constData()), [](IplImage* ptr) { cvReleaseImage(&ptr); });
	shared_ptr<IplImage> longImage(io.readImage(longReferenceFileBox->text().toAscii().constData()), [](IplImage* ptr) { cvReleaseImage(&ptr); });

	codec->setReferencePlane(shortImage, longImage);
  }

  return codec;
}

void SixFringeCodecOptionsController::selectFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select file to open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void SixFringeCodecOptionsController::loadReferenceValueChange(int checkState)
{
  if(Qt::Checked == checkState)
  {
	shortReferenceFileBox->setReadOnly(false);
	longReferenceFileBox->setReadOnly(false);

	shortReferenceFileChooseButton->setEnabled(true);
	longReferenceFileChooseButton->setEnabled(true);
  }
  else
  {
	shortReferenceFileBox->setReadOnly(true);
	longReferenceFileBox->setReadOnly(true);

	shortReferenceFileChooseButton->setEnabled(false);
	longReferenceFileChooseButton->setEnabled(false);
  }
}

void SixFringeCodecOptionsController::selectShortWavelengthFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select file to open", "/", "Image (*.png *.jpg)");

  if(!file.isEmpty())
  {
	shortReferenceFileBox->setText(file);
  }
}

void SixFringeCodecOptionsController::selectLongWavelengthFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select file to open", "/", "Image (*.png *.jpg)");

  if(!file.isEmpty())
  {
	longReferenceFileBox->setText(file);
  }
}

void SixFringeCodecOptionsController::_connectSignalsWithController(void)
{
  connect(loadReferenceCheckbox, SIGNAL(stateChanged(int)), this, SLOT(loadReferenceValueChange(int)));
  connect(sourceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
  connect(shortReferenceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectShortWavelengthFile()));
  connect(longReferenceFileChooseButton, SIGNAL(clicked()), this, SLOT(selectLongWavelengthFile()));
}