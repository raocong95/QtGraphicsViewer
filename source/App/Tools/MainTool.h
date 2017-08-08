#pragma once

#include <QGraphicsProxyWidget>

#include "Tool.h"
#include "MainToolView.h"
#include "MeshModel.h"

namespace Ui{class MainToolPanel;}
class DepthPeeler;
class Voxelizer;
class Optimizer;

class MainTool : public Tool
{
    Q_OBJECT

public:
    MainTool(Document* doc, QString id = "");
	~MainTool();

	void initialize() override;

public slots:
	// Ui    
	void resize(QSize newsize) override;
	void changeBkgBrightness(int brightness);

	// mesh
	void loadMesh();
	void saveMesh();
	void updateMeshTransform();  // transform mesh by using UI
	void applyMeshTransform();  // apply mesh transformation

    // test
    void test();

private:
	// ui
    MainToolView*			_view; // the main view for 3D content
    Ui::MainToolPanel*		_panel;
    QWidget*				_panelWidget;
	QGraphicsProxyWidget*	_panelProxy; // control panel on the left
};


