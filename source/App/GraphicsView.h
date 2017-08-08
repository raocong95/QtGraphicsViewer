#pragma once
#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event) override;

signals:
    void resized(QSize);
};
