#pragma once

#include <QGraphicsObject>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <Eigen/Core>
#include <QOpenGLFunctions_3_2_Core>

#include "MeshModel.h"
#include "ShaderRenderer.h"

#include "Plane.h"

namespace Eigen{ class Camera; class Trackball; }

class MainTool;

class MainToolView : public QGraphicsObject, public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

public:
    MainToolView(MainTool* tool);
    ~MainToolView();

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	void drawAxes();
	void drawGround();
	void drawMesh();

	void drawPoints();
	void drawPlanes();
	void drawDebug();

    QRectF rect;
    void setRect(const QRectF & newRect){ this->rect = newRect; }
    // this pure virtual function returns the bounding rect of this item
    QRectF boundingRect() const override { return this->rect; }

protected:
    Eigen::Camera* _camera;
    Eigen::Trackball* _trackball;
	QMatrix4x4 _cameraMatrix;
	QVector3D _cameraPos;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void wheelEvent(QGraphicsSceneWheelEvent *);
	void keyPressEvent(QKeyEvent *event);
    QPointF _buttonDownCursorPos, _mouseMoveCursorPos, _buttonUpCursorPos;
	Eigen::Vector3f _cameraLoc;
    bool _leftButtonDown, _rightButtonDown, _middleButtonDown;

public slots:
	void setMesh(MeshModel::Ptr mesh);

	void setPoints(QVector<QVector3D>& pnts, GLenum mode);
	void setPoints2(QVector<QVector3D>& pnts, GLenum mode);
	void setPlanes(QVector<Plane> planes);
	void resetGroundGrids();

private:
	QVector<QVector3D> getCompiledTriangles(Plane plane, float scale);

private:
	// main tool
	MainTool* _mainTool;

	// axes and ground
	ShaderRenderer::Ptr _axesRenderer, _groundRenderer;

	// mesh
	MeshModel::Ptr _meshModel;			
	ShaderRenderer::Ptr _meshRenderer;

	// points, lines
	QVector<QVector3D> _points, _points2;
	GLenum _pointsMode, _points2Mode;
	ShaderRenderer::Ptr _pointsRenderer, _points2Renderer;

	// planes
    QVector<Plane> _planes;
	ShaderRenderer::Ptr _planesRenderer, _planesOrientRenderer;

	// debug visual elements
	ShaderRenderer::Ptr _debugShaderRenderer;

public:
    bool _showGround, _showAxes;

	bool _showMesh, _showMeshWireframe;

	bool _showPoints, _showPlanes;

	bool _showDebugMesh;
};

