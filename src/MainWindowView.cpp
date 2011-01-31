#include "MainWindowView.h"

MainWindowView::MainWindowView(QMainWindow* parent) : QMainWindow(parent)
{	
	// Sets up the interface elements from Designer file
	setupUi(this);
	
        m_holoEncoder = new Holoencoder();
        m_holoDecoder = new Holodecoder(glWidget);
        glWidget->setNewGLContext(m_holoEncoder);
	
        glWidget->m_glDecoder = m_holoDecoder;
        glWidget->m_glEncoder = m_holoEncoder;
	
        QRect fileListInitial = fileList->geometry();
        QRect windowInitial = geometry();
        windowInitial.setWidth(windowInitial.width() - fileListInitial.width());
        fileListInitial.setWidth(0);
        fileList->setGeometry(fileListInitial);
        setGeometry(windowInitial);

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
        connect(modeView, SIGNAL(triggered()), controller, SLOT(viewMode()));
        connect(modeEncode, SIGNAL(triggered()), controller, SLOT(encodeMode()));

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
        connect(toolMapper, SIGNAL(mapper(int)), controller, SLOT(toolSelect(const int)));

        //	Connect the mode bar signals
        //	Need a mapper to map each signal to a tool type
        QSignalMapper* modeMapper = new QSignalMapper(this);
        modeMapper->setMapping(modeView, 0);
        modeMapper->setMapping(modeEncode, 1);

        //	Connect the tool signals to the mapper
        connect(modeView, SIGNAL(triggered()), modeMapper, SLOT(map()));
        connect(modeEncode, SIGNAL(triggered()), modeMapper, SLOT(map()));

        //	Connect the mapper signal to the controller
        connect(modeMapper, SIGNAL(mapped(int)), controller, SLOT(modeSelect(const int)));
}
