#pragma once

#include <QGraphicsObject>
#include <QPainter>

class Document;
class GraphicsScene;

class Tool : public QGraphicsObject
{
    Q_OBJECT

public:
    Tool(Document* doc, QString id = "");
    virtual ~Tool();

	// the graphics scene
	GraphicsScene* scene();

    // pure virtual functions from QGraphicsObject
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

	// interface of tool class
	virtual void initialize() = 0;
    QString getId();

public slots:
    virtual void resize(QSize newsize);

protected:
    QSize _size;
    QString _id;
    Document* _doc;
};
