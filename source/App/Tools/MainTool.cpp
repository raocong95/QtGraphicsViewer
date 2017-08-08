#include "MainTool.h"
#include "ui_MainToolPanel.h"
#include "GraphicsScene.h"
#include "Document.h"
#include "ParSingleton.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <QSlider>
#include <QTime>
#include <QDebug>

MainTool::MainTool(Document* doc, QString id) :
    Tool(doc, id)
{
	_view = nullptr;
	_panel = nullptr;
	_panelWidget = nullptr;
	_panelProxy = nullptr;
}

MainTool::~MainTool()
{
	delete _view;
	delete _panel;
	delete _panelProxy;
	// note that parent widget will delete their children
	// panelWidget has two parents, so we don't explicitly delete it
}

void MainTool::initialize()
{
	// call this function after adding the tool into the scene
	if (!scene()) return;

    // this tool occupies the entire view, so place at (0,0)
    this->setPos(0, 0);

	// place control panel
    _panelWidget = new QWidget();
    _panel = new Ui::MainToolPanel();
    _panel->setupUi(_panelWidget);
	auto ps = ParSingleton::instance();
    _panelWidget->setFixedWidth(ps->leftPanelWidth);

	_panelProxy = new QGraphicsProxyWidget(this); // create a proxy in the scene, default location = (0,0)
    _panelProxy->setWidget(_panelWidget); // wrap the panel with a proxy

    // the OpenGL view
    _view = new MainToolView(this);
    _view->setZValue(-100);

	{
		// Ui
		_panel->bkgBrightnessSlider->setValue(ParSingleton::instance()->bkgBrightness * 255);
		connect(_panel->bkgBrightnessSlider, SIGNAL(valueChanged(int)), SLOT(changeBkgBrightness(int)));

		// mesh
		connect(_panel->loadButton, SIGNAL(pressed()), SLOT(loadMesh()));
		connect(_panel->saveButton, SIGNAL(pressed()), SLOT(saveMesh()));
		connect(_panel->updateTransformButton, SIGNAL(pressed()), SLOT(updateMeshTransform()));
		connect(_panel->applyTransformButton, SIGNAL(pressed()), SLOT(applyMeshTransform()));

        // test
		connect(_panel->testButton, SIGNAL(pressed()), SLOT(test()));
	}

	// load default mesh
	//QString filename("C:\\PROJECTS\\pre-fab-print\\Code\\data\\bunny.off");
	//QString filename("C:\\PROJECTS\\pre-fab-print\\Code\\data\\couch.off");
	//QString filename("C:\\PROJECTS\\pre-fab-print\\Code\\data\\football_trophy_10\\0_modified.off");
	//_doc->loadMesh(filename);
	//_view->setMesh(_doc->_mesh);
}


void MainTool::resize(QSize newsize)
{
    // override this slot to handle layout of tool items
    Tool::resize(newsize);

    // place the panel at bottom left
    _panelProxy->setPos(0, newsize.height() - _panelWidget->height());

    // resize the OpenGL view
	auto ps = ParSingleton::instance();
    QRect vr(ps->leftPanelWidth, 0, newsize.width()- ps->leftPanelWidth, newsize.height());
    _view->setRect(vr);

	// force update
	scene()->update();
}

void MainTool::changeBkgBrightness(int brightness)
{
	ParSingleton::instance()->bkgBrightness = brightness / 255.0;
	// force update
	scene()->update();
}

void MainTool::loadMesh()
{
	auto ps = ParSingleton::instance();
	QString filename = QFileDialog::getOpenFileName(0, "Load mesh", ps->dataPath, "Mesh files (*.obj *.off);;All files(*.*)");
	if (filename.isEmpty()) return;

	_doc->loadMesh(filename);
	_view->setMesh(_doc->_mesh);

	scene()->displayMessage("Mesh loaded.", 1000);
}



void MainTool::saveMesh()
{
	auto ps = ParSingleton::instance();
	QString filename = QFileDialog::getSaveFileName(0, "Save mesh", ps->dataPath, "mesh file (*.*)");
	if (filename.isEmpty()) return;

	_doc->saveMesh(filename);
}

void MainTool::updateMeshTransform()
{
	auto mesh = _doc->_mesh;
	mesh->translate(_panel->tranX->value(), _panel->tranY->value(), _panel->tranZ->value());
	mesh->scale(_panel->scale->value());

	_panel->tranX->setValue(0);
	_panel->tranY->setValue(0);
	_panel->tranZ->setValue(0);
	_panel->scale->setValue(1);
}

void MainTool::applyMeshTransform()
{
	auto mesh = _doc->_mesh;
	mesh->applyTransform();
	_view->setMesh(mesh); // force update the mesh renderer
}


void MainTool::test()
{
    scene()->displayMessage("Hello World", 1000);
}

