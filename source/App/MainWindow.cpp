#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "GraphicsView.h"
#include "QDebug"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup the Graphics-View Framework
    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    scene = new GraphicsScene();
    ui->graphicsView->setScene(scene); // register the scene managing all 2D graphics items

    glwidget = new QOpenGLWidget();
	QSurfaceFormat format; format.setSamples(4);
	glwidget->setFormat(format);
	glwidget->setMouseTracking(true);
    ui->graphicsView->setViewport(glwidget); // enable OpenGL support

    // show the window so that the graphics view is correctly resized
    showMaximized();

    // initialization
    scene->initTools();

    // Default event processing
    QApplication::processEvents();
}

MainWindow::~MainWindow()
{
    delete ui;
	delete scene;
	delete glwidget;
}
