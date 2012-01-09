#include "MainWindowView.h"

MainWindowView::MainWindowView(QMainWindow* parent) : QMainWindow(parent)
{	
  // Sets up the interface elements from Designer file
  setupUi(this);

  captureController->setInfoBar(infoBar);

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
  connect(actionOpenXYZM, SIGNAL(triggered()), controller, SLOT(onOpenXYZM()));
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
