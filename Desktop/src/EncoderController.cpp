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
  encoderGLWidget->setGLContext(&m_encoder);
}

void EncoderController::exportCurrentFrame(void)
{
  QString fileName = QFileDialog::getSaveFileName(this, "Save File", "/", "Images (*.png)");

  if(!fileName.isEmpty())
  {
    ImageIO io;
    Texture holoimage = m_encoder.encode();
    io.saveTexture(fileName.toAscii().constData(), holoimage);
  }
}

void EncoderController::exportEntireVideo(QListWidget* fileList)
{
  Logger::logDebug("EncoderController - exportEntireVideo: Enter");

  QString fileName = QFileDialog::getSaveFileName(this, "Save File", "/", "Video (*.avi)");

  if(!fileName.isEmpty())
  {
      VideoIO io;
      bool canSaveFile = io.openSaveStream(fileName.toAscii().constData(), 512, 512, 30);

      if(canSaveFile)
      {
          XYZFileIO fileIO;

          //	Inform the user of the progress
          QProgressDialog progress("Encoding frames...", 0, 0, fileList->count(), this);

          Logger::logDebug("EncoderController - exportEntireVideo: Encoding frames");
          for(int itemNumber = 0; itemNumber < fileList->count(); itemNumber++)
          {
              //	Increase the progress
              progress.setValue(itemNumber);

              QListWidgetItem *item = fileList->item(itemNumber);

              AbstractMesh* currentMesh = fileIO.newMeshFromFile(item->text().toAscii().constData());
              m_encoder.setCurrentMesh(currentMesh);

              Texture holoimage = m_encoder.encode();
              io.saveStream(holoimage);
          }

          //	Last one done!
          progress.setValue(fileList->count());

          Logger::logDebug("EncoderController - exportEntireVideo: Encoding complete!");
          io.closeSaveStream();
      }
  }
}

void EncoderController::selectXYZM(const string &filename)
{
  XYZFileIO fileIO;
  AbstractMesh* currentMesh = fileIO.newMeshFromFile(filename);
  m_encoder.setCurrentMesh(currentMesh);
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

void EncoderController::selectSourceFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select source file to Open", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	sourceFileBox->setText(file);
  }
}

void EncoderController::selectDestinationFile(void)
{
  QString file = QFileDialog::getOpenFileName(this, "Select destination file to save to", "/", "Video (*.avi)");

  if(!file.isEmpty())
  {
	destFileBox->setText(file);
  }
}

void EncoderController::newDecoder(const QString& text)
{
}

void EncoderController::newEncoder(const QString& text)
{
  if(0 == QString(DepthCodec::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	  encoderOptionsStackedWidget->setCurrentWidget(depthMapOptions);
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
  QString sourceFilename = sourceFileBox->text();
  string str = sourceFilename.toStdString();
  decoder->openDecodeStream(decoderGLWidget, str);

  //  Setup encoder
  QString destFilename = destFileBox->text();
  string str2 = destFilename.toStdString();
  encoder->openEncodeStream(encoderGLWidget, str2, decoder->getDecodeStreamWidth(), decoder->getDecodeStreamHeight());

  //  As long as we have meshes decode and encode them
  MeshInterchange* mesh = decoder->decode();
  while(NULL != mesh)
  {
	// Indicate to the user the current progress
	encodingProgress->setValue(decoder->getDecodeStreamProgress() * 100);

    encoder->encode(*mesh);
    delete mesh;
    mesh = decoder->decode();
  }

  //  Close up
  encoder->closeEncodeStream();
  decoder->closeDecodeStream();

  //  Make sure our codec widgets are not pointing to anything
  encoderGLWidget->setGLContext(NULL);
  decoderGLWidget->setGLContext(NULL);

  //  Delete the encoder and decoder
  delete decoder;
  delete encoder;
}

void EncoderController::_connectSignalsWithController(void)
{
  connect(encodeButton,				SIGNAL(clicked()), this, SLOT(encode()));
  connect(sourceFileChooseButton,	SIGNAL(clicked()), this, SLOT(selectSourceFile()));
  connect(destFileChooseButton,		SIGNAL(clicked()), this, SLOT(selectDestinationFile()));
  connect(decoderComboBox,			SIGNAL(currentIndexChanged(const QString&)), this, SLOT(newDecoder(const QString&)));
  connect(encoderComboBox,			SIGNAL(currentIndexChanged(const QString&)), this, SLOT(newEncoder(const QString&)));
}

void EncoderController::_addCodecs(void)
{
  //  Add decoders
  decoderComboBox->addItem(QString(MultiWavelengthCodec::codecName().c_str()));
  decoderComboBox->addItem(QString(HolovideoCodec::codecName().c_str()));

  //  Add encoders
  encoderComboBox->addItem(QString(DepthCodec::codecName().c_str()));
}

Codec* EncoderController::_getEncoder(void)
{
  Codec* encoder = NULL;

  if(0 == QString(DepthCodec::codecName().c_str()).compare(encoderComboBox->currentText()))
  {
	//	Create and initalize a new depth codec
	encoder = depthMapOptions->getDepthmapCodec();
  }

  return encoder;
}

Codec* EncoderController::_getDecoder(void)
{
  Codec* decoder = NULL;

  if(0 == QString(MultiWavelengthCodec::codecName().c_str()).compare(decoderComboBox->currentText()))
  {
	decoder = new MultiWavelengthCodec();
  }
  else
  {
	//	Unknown decoder
	Logger::logError("EncoderController - encode: Unable to get a valid decoder");
  }

  return decoder;
}