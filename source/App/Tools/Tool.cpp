#include "Tool.h"
#include "GraphicsScene.h"

Tool::Tool(Document* doc,QString id /*= ""*/) :
    _id(id), _doc(doc)
{
    if (_id.isEmpty()) _id = staticMetaObject.className();
}

Tool::~Tool()
{

}

GraphicsScene* Tool::scene()
{
	return (GraphicsScene*)QGraphicsObject::scene();
}

QRectF Tool::boundingRect() const
{
    // a tool occupies the entire view region
    return QRectF(0, 0, _size.width(), _size.height());
}

void Tool::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(widget);
	return;

	// debug
    painter->setPen(QPen(Qt::red, 5));
    painter->drawRect(QRect(0, 0, _size.width(), _size.height()));
}

QString Tool::getId()
{
    return _id;
}

void Tool::resize(QSize newsize)
{
    // by default, the tool occupies the entire GraphicsView
    _size = newsize;
}

