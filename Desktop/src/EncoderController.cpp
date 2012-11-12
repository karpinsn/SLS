#include "EncoderController.h"

EncoderController::EncoderController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
  _connectSignalsWithController();
  _addCodecs();
}

void EncoderController::init(void)
{
}

void EncoderController::newDecoder(const QString& text)
{
  if(0 == QString(NineFringeDecoder::codecName().c_str()).compare(text))
  {
    decoderOptionsStackedWidget->setCurrentWidget(nineFringeOptions);
  }
  else if(0 == QString(SixFringeDecoder::codecName().c_str()).compare(text))
  {
    decoderOptionsStackedWidget->setCurrentWidget(sixFringeOptions);
  }
  else if(0 == QString(HolovideoDecoder::codecName().c_str()).compare(text))
  {
	decoderOptionsStackedWidget->setCurrentWidget(holovideoOptions);
  }
  else if(0 == QString(XYZMDecoder::codecName().c_str()).compare(text))
  {
    decoderOptionsStackedWidget->setCurrentWidget(xyzmOptions);
  }
  else
  {
    decoderOptionsStackedWidget->setCurrentWidget(defaultDecoderOptions);
  }
}

void EncoderController::newEncoder(const QString& text)
{
  if(0 == QString(DepthEncoder::codecName().c_str()).compare(text))
  {
    encoderOptionsStackedWidget->setCurrentWidget(depthMapOptions);
  }
  else if(0 == QString(HolovideoEncoder::codecName().c_str()).compare(text))
  {
	encoderOptionsStackedWidget->setCurrentWidget(holovideoEncodeOptions);
  }
  else if(0 == QString(HoloimageEncoder::codecName().c_str()).compare(text))
  {
	encoderOptionsStackedWidget->setCurrentWidget(holoimageEncodeOptions);
  }
  else
  {
    decoderOptionsStackedWidget->setCurrentWidget(defaultEncoderOptions);
  }

  if(this->isVisible())
  {
	_previewEncoding();
  }
}

void EncoderController::encode(void)
{
  shared_ptr<Codec> decoder = _getDecoder();
  shared_ptr<Codec> encoder = _getEncoder();

  if(nullptr == decoder || nullptr == encoder)
  {
	//	Invalid codec
	return;
  }

  //  Setup decoder: Make sure our calls go to the decoder widget
  decoderGLWidget->makeCurrent();
  decoder->openCodec(decoderGLWidget);

  //  Setup encoder: Make sure our calls go to the decoder widget
  encoderGLWidget->makeCurrent();
  encoder->setWidth(decoder->getWidth());
  encoder->setHeight(decoder->getHeight());
  encoder->openCodec(encoderGLWidget);

  //  As long as we have meshes decode and encode them
  shared_ptr<MeshInterchange> mesh = decoder->process(nullptr);
  while(mesh && !mesh->isEmpty())
  {
	// Indicate to the user the current progress
    encodingProgress->setValue(decoder->getStreamLocation() * 100);

    encoder->process(mesh);
    mesh = decoder->process(nullptr);
  }

  //  Close up
  encoder->closeCodec();
  decoder->closeCodec();

  //  Make sure our codec widgets are not pointing to anything
  encoderGLWidget->setEncodingContext(nullptr);
  decoderGLWidget->setDecodingContext(nullptr);
}

void EncoderController::_connectSignalsWithController(void)
{
  connect(encodeButton,				SIGNAL(clicked()), this, SLOT(encode()));
  connect(decoderComboBox,			SIGNAL(currentIndexChanged(const QString&)), this, SLOT(newDecoder(const QString&)));
  connect(encoderComboBox,			SIGNAL(currentIndexChanged(const QString&)), this, SLOT(newEncoder(const QString&)));
}

void EncoderController::_addCodecs(void)
{
  //  Add decoders
  decoderComboBox->addItem(QString(NineFringeDecoder::codecName().c_str()));
  decoderComboBox->addItem(QString(SixFringeDecoder::codecName().c_str()));
  decoderComboBox->addItem(QString(HolovideoDecoder::codecName().c_str()));
  decoderComboBox->addItem(QString(XYZMDecoder::codecName().c_str()));

  //  Add encoders
  encoderComboBox->addItem(QString(DepthEncoder::codecName().c_str()));
  encoderComboBox->addItem(QString(HolovideoEncoder::codecName().c_str()));
  encoderComboBox->addItem(QString(HoloimageEncoder::codecName().c_str()));
}

void EncoderController::_previewEncoding(void)
{
  shared_ptr<Codec> decoder = _getDecoder();
  shared_ptr<Codec> encoder = _getEncoder();

  if(nullptr == decoder || nullptr == encoder)
  {
	//	Invalid codec
	return;
  }
  
  //  Make sure our calls go to the decoder widget
  decoderGLWidget->makeCurrent();
  decoder->openCodec(decoderGLWidget);

  //  Now switch to the encoder widget
  encoderGLWidget->makeCurrent();
  encoder->setWidth(decoder->getWidth());
  encoder->setHeight(decoder->getHeight());
  encoder->openCodec(encoderGLWidget);

  //  Set the size back to whatever the preview window is
  encoderGLWidget->reinit(encoderGLWidget->size().width(), encoderGLWidget->size().height());

  shared_ptr<MeshInterchange> mesh = decoder->previewProcess(nullptr);

  if(mesh)
  {
    encoder->previewProcess(mesh);
  }

  //  Close up
  encoder->closeCodec();
  decoder->closeCodec();
}

shared_ptr<Codec> EncoderController::_getEncoder(void)
{
  shared_ptr<Codec> encoder = nullptr;

  if(0 == QString(DepthEncoder::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	//	Create and initalize a new depth codec
	encoder = depthMapOptions->getCodec();
  }
  else if(0 == QString(HolovideoEncoder::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	encoder = holovideoEncodeOptions->getCodec();
  }
  else if(0 == QString(HoloimageEncoder::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	encoder = holoimageEncodeOptions->getCodec();
  }
  else
  {
	//	Unknown decoder
	Logger::logError("EncoderController - encode: Unable to get a valid encoder");
  }

  return encoder;
}

shared_ptr<Codec> EncoderController::_getDecoder(void)
{
  shared_ptr<Codec> decoder = nullptr;

  if(0 == QString(NineFringeDecoder::codecName().c_str()).compare(decoderComboBox->currentText()))
  {
	decoder = nineFringeOptions->getCodec();
  }
  else if(0 == QString(SixFringeDecoder::codecName().c_str()).compare(decoderComboBox->currentText()))
  {
    decoder = sixFringeOptions->getCodec();
  }
  else if(0 == QString(HolovideoDecoder::codecName().c_str()).compare(decoderComboBox->currentText()))
  {
	decoder = holovideoOptions->getCodec();
  }
  else if(0 == QString(XYZMDecoder::codecName().c_str()).compare(decoderComboBox->currentText()))
  {
    //  Return the XYZMCodec
    decoder = xyzmOptions->getCodec();
  }
  else
  {
	//	Unknown decoder
	Logger::logError("EncoderController - encode: Unable to get a valid decoder");
  }

  return decoder;
}
