#include "MainWindowView.h"

MainWindowView::MainWindowView(QMainWindow* parent) : QMainWindow(parent)
{	
	// Sets up the interface elements from Designer file
	setupUi(this);
	
	// Setup the encoder widget
	horizontalLayout_3->removeWidget(glView);
	glView->setParent(0);
	glView = NULL;
	
	m_holoEncoder = new Holoencoder();
	m_glWidget = new OpenGLWidget(this, m_holoEncoder, QColor::fromRgb(0, 0, 0, 0));
	m_glWidget->setMinimumSize(512, 512);
	m_glWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	horizontalLayout_3->insertWidget(0, m_glWidget);
	m_holoDecoder = new MockHolodecoder(m_glWidget);
	
	m_glWidget->m_glDecoder = m_holoDecoder;
	m_glWidget->m_glEncoder = m_holoEncoder;
	
	_initTopMenu();
}

MainWindowView::~MainWindowView()
{
}

void MainWindowView::_initTopMenu()
{
	QActionGroup* group = new QActionGroup(this);
	group->addAction(actionPan);
	group->addAction(actionRotate);
	group->addAction(actionZoom);
}

void MainWindowView::connectSignalsWithController(QObject* controller)
{
	// Connect the interface events (signals) to the controller class object
	connect(encodeButton, SIGNAL(clicked()), controller, SLOT(onEncodeButton()));
	connect(fileList, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), controller, SLOT(selectXYZM(QListWidgetItem*, QListWidgetItem*)));
	connect(actionOpenXYZM, SIGNAL(triggered()), controller, SLOT(onOpenXYZM()));
	connect(actionOpen_Holovideo, SIGNAL(triggered()), controller, SLOT(playVideo()));
	connect(actionOpen_Holoimage, SIGNAL(triggered()), controller, SLOT(openHoloImage()));

	//	Connect the tool bar signals
	//	Need a mapper to map each signal to a tool type
	QSignalMapper* mapper = new QSignalMapper(this);
	mapper->setMapping(actionPan, Camera::PAN_MODE);
	mapper->setMapping(actionRotate, Camera::ROTATE_MODE);
	mapper->setMapping(actionZoom, Camera::ZOOM_MODE);
	
	//	Connect the tool signals to the mapper
	connect(actionPan, SIGNAL(triggered()), mapper, SLOT(map()));
	connect(actionRotate, SIGNAL(triggered()), mapper, SLOT(map()));
	connect(actionZoom, SIGNAL(triggered()), mapper, SLOT(map()));
	
	//	Connect the mapper signal to the controller
	connect(mapper, SIGNAL(mapped(int)), controller, SLOT(toolSelect(const int)));
}
