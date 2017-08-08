#include "GraphicsView.h"
#include <QGraphicsProxyWidget>
#include <QSettings>
#include <QKeyEvent>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    // disable scroll bars
    // force the graphics view to be the size of the central widget in the mainwindow
    this->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    emit(resized(event->size()));
}
