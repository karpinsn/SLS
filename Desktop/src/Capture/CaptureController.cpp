#include "CaptureController.h"

CaptureController::CaptureController(QWidget* parent) : QWidget(parent)
{
  m_gl3DContext	  = make_shared<NineFringeCapture>();
  m_camera		  = make_shared<CameraCapture>();
  m_frameCapture  = make_shared<FrameCapture>();
  m_buffer		  = make_shared<ImageBuffer>();

  setupUi(this);                    //  Creates the UI objects
  _connectSignalsWithController();  //  Connects the UI objects to the slots in the controller
  _readSettings();                  //  Reads the persisted settings

  m_dropFrame = false;
  m_fpsLabel.setText(QString(""));
  m_3dpsLabel.setText(QString(""));
  m_infoBar = nullptr;
}

CaptureController::~CaptureController()
{
}

void CaptureController::showEvent(QShowEvent *event)
{  
  CaptureTypeSelectDialog dialog;
  m_gl3DContext = dialog.getCaptureType();
  if(nullptr == m_gl3DContext)
  {
	throw new exception("No capture type selected");
  }

  captureGLWidget->setGLContext(m_gl3DContext.get());
  m_gl3DContext->init();

  //  Connect to camera
  m_frameCapture->start();
  m_frameRateTimer.start(1000);

  //  Display the current FPS
  m_infoBar->addPermanentWidget(&m_fpsLabel);
  m_infoBar->addPermanentWidget(&m_3dpsLabel);

  captureGLWidget->updateScene();
}

void CaptureController::hideEvent(QHideEvent *)
{
  //  Remove the FPS counter
  if(nullptr != m_infoBar)
  {
    m_infoBar->removeWidget(&m_fpsLabel);
    m_infoBar->removeWidget(&m_3dpsLabel);
  }

  //  Stop our camera capture
  if(nullptr != m_camera)
  {
	m_camera->stop();
  }
}

void CaptureController::init(void)
{
  m_camera->init(m_buffer.get());
  m_frameCapture->init(m_buffer.get());
  captureGLWidget->setGLContext(m_gl3DContext.get());
}

void CaptureController::setInfoBar(QStatusBar* infoBar)
{
  m_infoBar = infoBar;
}

void CaptureController::captureReference(void)
{
  if(m_camera->hasCamera())
  {
	//	Capture the reference plane from the camera
	m_gl3DContext->captureReferencePlane();
  }
  else
  {
	//	We dont have a camera so user wants to load a reference plane from a file
	m_gl3DContext->loadReferencePlane(this, &CaptureController::_newFrameFromFileCallback);
  }
}

void CaptureController::cameraSelectMode(int mode)
{
  m_gl3DContext->cameraSelectMode(mode);
}

void CaptureController::connectCamera(void)
{
  m_infoBar->showMessage("Connecting to camera...");

  CameraConnectDialog dialog;
  unique_ptr<lens::Camera> camera(dialog.getCamera());

  //  Make sure that we have a new camera
  if(nullptr != camera)
  {
	//	Initialize our camera
	camera->init();

    //  Reinitalize OpenGL stuff
    m_gl3DContext->resizeInput(camera->getWidth(), camera->getHeight());
    
	//	Transfer ownership and start up
	m_camera->setCamera(::move(camera));
    m_camera->start();
    m_infoBar->showMessage("Connected to the camera");
  }
  else
  {
    m_infoBar->showMessage("Not connected");
  }
}

void CaptureController::disconnectCamera(void)
{
  m_camera->stop();
  m_infoBar->showMessage("Camera stopped");
}

void CaptureController::dropFrame(void)
{
  m_dropFrame = true;
}

void CaptureController::newGammaValue(double gammaValue)
{
  //  Set the new gamma value
  m_gl3DContext->setGammaCutoff(gammaValue);

  //  Persist the new gamma value
  m_settings.setValue(SettingsGammaValue, gammaValue);
}

void CaptureController::newScalingFactor(double scalingFactor)
{
  m_gl3DContext->setScalingFactor(scalingFactor);

  //  Persist the new scaling factor
  m_settings.setValue(SettingsScalingFactor, scalingFactor);
}

void CaptureController::newViewMode(QString viewMode)
{
  if(0 == viewMode.compare(QString("3D")))
  {
	//	TODO - Comeback and fix this
    //m_gl3DContext->show3D();
  }
  else if(0 == viewMode.compare(QString("Phase")))
  {
	//	TODO - Comeback and fix this
    //m_gl3DContext->showPhase();
  }
}

void CaptureController::updateFPS(void)
{
  double frameRate = m_gl3DContext->getFrameRate();
  QString frameRateMessage = QString("FPS: ");
  frameRateMessage.append(QString("%1").arg(frameRate, 0, 'f', 3));
  m_fpsLabel.setText(frameRateMessage);

  double threeDRate = m_gl3DContext->get3DRate();
  QString threeDRateMessage = QString("3DPS: ");
  threeDRateMessage.append(QString("%1").arg(threeDRate, 0, 'f', 3));
  m_3dpsLabel.setText(threeDRateMessage);
}

void CaptureController::newFrame(IplImage *frame)
{
  if(!m_dropFrame)  //  If we dont drop a frame then process it
  {
    IplImage *im_gray = frame;
    bool releaseGray = false;

    if(frame->nChannels > 1)
    {
      im_gray = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,1);
      cvCvtColor(frame, im_gray, CV_RGB2GRAY);
      releaseGray = true;
    }

    if(m_gl3DContext->newImage(im_gray))
    {
      captureGLWidget->updateScene();
    }

    if(releaseGray)
    {
      cvReleaseImage(&im_gray);
    }

    cvReleaseImage(&frame);
  }
  else  // Drop a frame
  {
    m_dropFrame = false;  //  We only want to drop one frame so set it back
  }
}

void CaptureController::save(void)
{
	if(nullptr == m_outputStream)
	{
		//	Create a codec and outputStream
		shared_ptr<Holoencoder> codec(new Holoencoder());
		codec->init(576, 576);
		shared_ptr<IOutputStream> outStream(new FileOutputStream());

		//	Create the save stream and open it
		m_outputStream = shared_ptr<SaveStream>(new SaveStream());
		m_outputStream->open(codec, outStream);

		//	Send the save stream to our current context
		m_gl3DContext->setSaveStream(m_outputStream);
	}
	else
	{
		//	Clear out the save stream and close it down
		m_gl3DContext->setSaveStream(nullptr);
		m_outputStream->close();
		m_outputStream = nullptr;
	}
}

shared_ptr<IplImage> CaptureController::_newFrameFromFileCallback(void* callbackInstance)
{
  CaptureController* controller = (CaptureController*) callbackInstance;
  return controller->_newFrameFromFile();
}

shared_ptr<IplImage> CaptureController::_newFrameFromFile(void)
{
  QString file = QFileDialog::getOpenFileName(nullptr, "Select image to Open", "/", "Images (*.png *.jpg)");

  if(!file.isEmpty())
  {
	ImageIO io;
	return shared_ptr<IplImage>(io.readImage(file.toAscii().constData()), [](IplImage* ptr) { cvReleaseImage(&ptr); });
  }

  return nullptr;
}

void CaptureController::_connectSignalsWithController(void)
{
  connect(m_frameCapture.get(), SIGNAL(newFrame(IplImage*)),		  this, SLOT(newFrame(IplImage*)));
  connect(openCameraButton,		SIGNAL(clicked()),                    this, SLOT(connectCamera()));
  connect(closeCameraButton,	SIGNAL(clicked()),                    this, SLOT(disconnectCamera()));
  connect(calibrateButton,		SIGNAL(clicked()),                    this, SLOT(captureReference()));
  connect(dropFrameButton,		SIGNAL(clicked()),                    this, SLOT(dropFrame()));
  connect(gammaBox,				SIGNAL(valueChanged(double)),         this, SLOT(newGammaValue(double)));
  connect(scalingFactorBox,		SIGNAL(valueChanged(double)),         this, SLOT(newScalingFactor(double)));
  connect(viewModeBox,			SIGNAL(currentIndexChanged(QString)), this, SLOT(newViewMode(QString)));
  connect(&m_frameRateTimer,	SIGNAL(timeout()),                    this, SLOT(updateFPS()));
  connect(saveButton,			SIGNAL(clicked()),					  this, SLOT(save()));
}

void CaptureController::_readSettings(void)
{
  //  Read in the settings file and set the settings
  gammaBox->setValue(         m_settings.value(QString("CaptureGammaValue"),    .10).toDouble());
  scalingFactorBox->setValue( m_settings.value(QString("CaptureScalingFactor"), .05).toDouble());
}
