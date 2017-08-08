#include "GraphicsScene.h"
#include "GraphicsView.h"

#include "Document.h"
#include "ModifiersPanel.h"
#include "ui_ModifiersPanel.h"
#include "Tools/Tool.h"
#include "Tools/MainTool.h"
#include "ParSingleton.h"

#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>


GraphicsScene::GraphicsScene()
{
}

GraphicsScene::~GraphicsScene()
{
    if (_document) delete _document;
    if (_modifiersPanel) delete _modifiersPanel;
    for(auto&& t : _tools) if (t) delete t;
}

void GraphicsScene::initTools()
{
	// call this function after setting up the scene-view system
	if (!view()) return;

	// dock the modifiersPanel at top left
	auto ps = ParSingleton::instance();
	_modifiersPanel = new ModifiersPanel();
	_modifiersPanel->setFixedWidth(ps->leftPanelWidth);
    addWidget(_modifiersPanel)->setPos(0, 0);

	// document holding global data
	_document = new Document(this);

	// the main tool
	{
        MainTool* mainTool = new MainTool(_document, "Demo Tool");
        addItem(mainTool); // add the tool into the scene before customize

        mainTool->initialize();
        mainTool->resize(view()->size());
        mainTool->setVisible(true); // hide/show the tool by default
		_tools.push_back(mainTool);

        // by default, the tool occupies the entire GraphicsView
        mainTool->connect(view(), SIGNAL(resized(QSize)), SLOT(resize(QSize)));
		connect(_modifiersPanel->ui->demoToolButton, &QPushButton::pressed, [&]() {
			showTool("Demo Tool");
		});
	}

    displayMessage("Welcome to the graphics viewer", 1000);
}

void GraphicsScene::showTool(QString toolname)
{
    // show the tool with matching toolname while hide others
    for(auto t : _tools){
        t->setVisible(t->getId() == toolname);
    }

    displayMessage(toolname + " is enabled", 1000);

    update();
}


void GraphicsScene::displayMessage(QString message, int time)
{    
    auto textItem = addText(message, QFont("SansSerif", 24));
    textItem->setDefaultTextColor(Qt::white);
    textItem->setZValue(500);

    if (view())
    {
        auto textRect = textItem->boundingRect();
        textItem->setPos(view()->width() * 0.5 - textRect.width() * 0.5,
                         view()->height() * 0.5 - textRect.height() * 0.5);
    }

    QTimer::singleShot(time, [=]{
        removeItem(textItem);
        update();
    });
}

GraphicsView *GraphicsScene::view()
{
	return (GraphicsView *)views().front();
}

void GraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    QGraphicsScene::wheelEvent(event);

    // block graphics view from receiving wheel events
    event->accept();
}
