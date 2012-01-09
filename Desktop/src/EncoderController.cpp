#include "EncoderController.h"

EncoderController::EncoderController(QWidget* parent) : QWidget(parent)
{
  setupUi(this);
  _connectSignalsWithController();
  _addCodecs();
}

EncoderController::~EncoderController()
{
}

void EncoderController::init(void)
{
}

void EncoderController::_updateGL(void)
{
  Logger::logDebug("EncoderController - _updateGL: Enter");

  OpenGLWidget* glContext = findChild<OpenGLWidget*>(QString::fromUtf8("encoderGLWidget"));

  if(NULL != glContext)
  {
    glContext->updateScene();
  }
  else
  {
    Logger::logError("EncoderController - _updateGL: Unable to find OpenGL Widget");
  }
}

void EncoderController::newDecoder(const QString& text)
{
  if(0 == QString(MultiWavelengthDecoder::codecName().c_str()).compare(text))
  {
    decoderOptionsStackedWidget->setCurrentWidget(multiWavelengthOptions);
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
  Codec* decoder = _getDecoder();
  Codec* encoder = _getEncoder();

  if(NULL == decoder || NULL == encoder)
  {
	//	Invalid codec
	return;
  }

  //  Setup decoder
  decoder->openCodec(decoderGLWidget);
  encoder->openCodec(encoderGLWidget);

  //  As long as we have meshes decode and encode them
  MeshInterchange* mesh = new MeshInterchange();
  decoder->process(mesh);
  while(NULL != mesh && !mesh->isEmpty())
  {
	// Indicate to the user the current progress
    encodingProgress->setValue(decoder->getStreamLocation() * 100);

    encoder->process(mesh);
	
	//	TODO Comeback and fix this
	delete mesh->getMesh();
	mesh->setMesh(NULL);

    decoder->process(mesh);
  }

  delete mesh;

  //  Close up
  encoder->closeCodec();
  decoder->closeCodec();

  //  Make sure our codec widgets are not pointing to anything
  encoderGLWidget->setGLContext(NULL);
  decoderGLWidget->setGLContext(NULL);
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
  decoderComboBox->addItem(QString(MultiWavelengthDecoder::codecName().c_str()));
  decoderComboBox->addItem(QString(HolovideoDecoder::codecName().c_str()));
  decoderComboBox->addItem(QString(XYZMDecoder::codecName().c_str()));

  //  Add encoders
  encoderComboBox->addItem(QString(DepthEncoder::codecName().c_str()));
  encoderComboBox->addItem(QString(HolovideoEncoder::codecName().c_str()));
}

void EncoderController::_previewEncoding(void)
{
  Codec* decoder = _getDecoder();
  Codec* encoder = _getEncoder();

  if(NULL == decoder || NULL == encoder)
  {
	//	Invalid codec
	return;
  }

  decoder->openCodec(decoderGLWidget);
  encoder->openCodec(encoderGLWidget);

  MeshInterchange* mesh = new MeshInterchange();
  decoder->process(mesh);

  if(NULL != mesh)
  {
    encoder->previewProcess(mesh);
  }

  //  Close up
  encoder->closeCodec();
  decoder->closeCodec();
}

Codec* EncoderController::_getEncoder(void)
{
  Codec* encoder = NULL;

  if(0 == QString(DepthEncoder::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	//	Create and initalize a new depth codec
	encoder = depthMapOptions->getCodec();
  }
  else if(0 == QString(HolovideoEncoder::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	encoder = holovideoEncodeOptions->getCodec();
  }
  else
  {
	//	Unknown decoder
	Logger::logError("EncoderController - encode: Unable to get a valid encoder");
  }

  return encoder;
}

Codec* EncoderController::_getDecoder(void)
{
  Codec* decoder = NULL;

  if(0 == QString(MultiWavelengthDecoder::codecName().c_str()).compare(decoderComboBox->currentText()))
  {
	decoder = multiWavelengthOptions->getCodec();
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
