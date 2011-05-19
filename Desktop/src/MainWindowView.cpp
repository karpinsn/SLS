#include "MainWindowView.h"

MainWindowView::MainWindowView(QMainWindow* parent) : QMainWindow(parent)
{	
  // Sets up the interface elements from Designer file
  setupUi(this);

  //m_holoEncoder = new Holoencoder();
  //m_holoDecoder = new Holodecoder();
  //glWidget->setNewGLContext(m_holoEncoder);

  //glWidget->m_glDecoder = m_holoDecoder;
  //glWidget->m_glEncoder = m_holoEncoder;



  QRect fileListInitial = fileList->geometry();
  QRect windowInitial = geometry();
  windowInitial.setWidth(windowInitial.width() - fileListInitial.width());
  fileListInitial.setWidth(0);
  fileList->setGeometry(fileListInitial);
  setGeometry(windowInitial);

  _initControllers();
  _initTopMenu();
}

MainWindowView::~MainWindowView()
{
}

void MainWindowView::_initTopMenu()
{
  QActionGroup* toolGroup = new QActionGroup(this);
  toolGroup->addAction(actionPan);
  toolGroup->addAction(actionRotate);
  toolGroup->addAction(actionZoom);

  QActionGroup* modeGroup = new QActionGroup(this);
  modeGroup->addAction(modeView);
  modeGroup->addAction(modeEncode);
  modeGroup->addAction(modeCapture);
}

void MainWindowView::_initControllers(void)
{
  viewController->init();
  encodeController->init();
  captureController->init();
}

void MainWindowView::connectSignalsWithController(QObject* controller)
{
  // Connect the interface events (signals) to the controller class object
  connect(fileList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), controller, SLOT(selectXYZM(QListWidgetItem*, QListWidgetItem*)));
  connect(actionOpenXYZM, SIGNAL(triggered()), controller, SLOT(onOpenXYZM()));
  connect(actionExportSingleFrame, SIGNAL(triggered()), controller, SLOT(exportSingleFrame()));
  connect(actionExportEntireVideo, SIGNAL(triggered()), controller, SLOT(exportEntireVideo()));
  connect(actionOpen_Holovideo, SIGNAL(triggered()), controller, SLOT(playVideo()));
  connect(actionOpen_Holoimage, SIGNAL(triggered()), controller, SLOT(openHoloImage()));
  connect(actionCalibrateRefrence, SIGNAL(triggered()), captureController, SLOT(captureReference()));

  connect(modeView, SIGNAL(triggered()), controller, SLOT(viewMode()));
  connect(modeEncode, SIGNAL(triggered()), controller, SLOT(encodeMode()));
  connect(modeCapture, SIGNAL(triggered()), controller, SLOT(captureMode()));

  //	Connect the tool bar signals
  //	Need a mapper to map each signal to a tool type
  QSignalMapper* toolMapper = new QSignalMapper(this);
  toolMapper->setMapping(actionPan, Camera::PAN_MODE);
  toolMapper->setMapping(actionRotate, Camera::ROTATE_MODE);
  toolMapper->setMapping(actionZoom, Camera::ZOOM_MODE);

  //	Connect the tool signals to the mapper
  connect(actionPan, SIGNAL(triggered()), toolMapper, SLOT(map()));
  connect(actionRotate, SIGNAL(triggered()), toolMapper, SLOT(map()));
  connect(actionZoom, SIGNAL(triggered()), toolMapper, SLOT(map()));

  //	Connect the mapper signal to the controller
  connect(toolMapper, SIGNAL(mapped(int)), controller, SLOT(toolSelect(int)));
}

void MainWindowView::showFileList(void)
{
  QSize fileListSize = fileList->maximumSize();

  //  Need to change the mode
  fileListAnimation = new QPropertyAnimation(fileList, "maximumSize");
  fileListAnimation->setDuration(500);
  fileListAnimation->setStartValue(QSize(fileListSize.width(), fileListSize.height()));
  fileListAnimation->setEndValue(QSize(200, fileListSize.height()));
  fileListAnimation->setEasingCurve(QEasingCurve::InOutCubic);

  QRect windowStart = geometry();
  QRect windowEnd = windowStart;
  //  There shouldnt be any width but just incase
  windowEnd.setWidth(windowStart.width() + (200 - fileListSize.width()));

  mainWindowAnimation = new QPropertyAnimation(this, "geometry");
  mainWindowAnimation->setDuration(500);
  mainWindowAnimation->setStartValue(windowStart);
  mainWindowAnimation->setEndValue(windowEnd);
  mainWindowAnimation->setEasingCurve(QEasingCurve::InOutCubic);

  animationGroup = new QSequentialAnimationGroup();
  animationGroup->addAnimation(mainWindowAnimation);
  animationGroup->addAnimation(fileListAnimation);
  animationGroup->start();
}

void MainWindowView::hideFileList(void)
{
  QSize fileListSize = fileList->maximumSize();

  //  Need to change the mode
  fileListAnimation = new QPropertyAnimation(fileList, "maximumSize");
  fileListAnimation->setDuration(500);
  fileListAnimation->setStartValue(QSize(fileListSize.width(), fileListSize.height()));
  fileListAnimation->setEndValue(QSize(0, fileListSize.height()));
  fileListAnimation->setEasingCurve(QEasingCurve::InOutCubic);

  QRect windowStart = geometry();
  QRect windowEnd = windowStart;
  windowEnd.setWidth(windowStart.width() - fileListSize.width());

  mainWindowAnimation = new QPropertyAnimation(this, "geometry");
  mainWindowAnimation->setDuration(500);
  mainWindowAnimation->setStartValue(windowStart);
  mainWindowAnimation->setEndValue(windowEnd);
  mainWindowAnimation->setEasingCurve(QEasingCurve::InOutCubic);

  animationGroup = new QSequentialAnimationGroup();
  animationGroup->addAnimation(fileListAnimation);
  animationGroup->addAnimation(mainWindowAnimation);
  animationGroup->start();
}
