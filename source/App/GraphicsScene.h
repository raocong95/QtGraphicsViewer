#pragma once

#include <QGraphicsScene>
#include <QVector>


class Document;
class Tool;
class GraphicsView;
class ModifiersPanel;

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene();
    ~GraphicsScene();

	void initTools();
    void showTool(QString toolname);

    // helper functions
    void displayMessage(QString message, int time = 3000);
	GraphicsView* view();


protected:
    void wheelEvent(QGraphicsSceneWheelEvent *event);

private:
    ModifiersPanel* _modifiersPanel;
    Document* _document;
    QVector<Tool*> _tools;
};

