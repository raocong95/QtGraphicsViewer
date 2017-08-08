#include "MainToolView.h"
#include "Camera.h"
#include "GraphicsView.h"
#include "GraphicsScene.h"
#include "MeshModel.h"
#include "ColorMap.h"
#include "ParSingleton.h"
#include "MainTool.h"

#include <QKeyEvent>
#include <QFileDialog>
#include <QDebug>

MainToolView::MainToolView(MainTool* tool)
    :QGraphicsObject(tool), _mainTool(tool)
{
	initializeOpenGLFunctions();

	// Enable keyboard
	this->setFlags(QGraphicsItem::ItemIsFocusable);

	// Create camera & trackball
	{
		_camera = new Eigen::Camera();
		_trackball = new Eigen::Trackball();
		_trackball->setCamera(_camera);
	}

    // toggle options
	_showGround = true;	_showAxes = false;
	_showMesh = true; _showMeshWireframe = true;
	_showPoints = false;
	_showPlanes = false;
	_showDebugMesh = false;
}



MainToolView::~MainToolView()
{
    delete _camera;
    delete _trackball;
}

void MainToolView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);

    // Check OpenGL widget
    auto glwidget = (QOpenGLWidget*)widget;
	if (!glwidget)  return;

	// Camera matrix
	_camera->setViewport(rect.width(), rect.height());
	Eigen::Matrix4f p = _camera->projectionMatrix();
	Eigen::Matrix4f v = _camera->viewMatrix().matrix();
	p.transposeInPlace(); v.transposeInPlace();
	_cameraMatrix = QMatrix4x4(p.data()) * QMatrix4x4(v.data());
	_cameraPos = QVector3D(_camera->position().x(), _camera->position().y(), _camera->position().z());

    // Begin drawing 3D
    painter->beginNativePainting();

	// viewport
	auto c = ParSingleton::instance()->bkgBrightness;
	glViewport(rect.left(), 0, rect.width(), rect.height());
	glClearColor(c, c, c, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_MULTISAMPLE); /// Antialiasing

    // ground and axis
	drawAxes();
	drawGround();

	// main content
	drawMesh();

	// debug
	drawPoints();
	drawPlanes();
	drawDebug();

    // End 3D drawing
    painter->endNativePainting();
	//painter->setPen(QPen(Qt::red, 5));
	//painter->drawRect(rect);
}

void MainToolView::drawAxes()
{
	if (_showAxes)
	{
		if (!_axesRenderer)
		{
			int length = 3;
			QVector<QVector3D> Vs;  Vs 
				<< QVector3D(0, 0, 0) << length * QVector3D(1, 0, 0)
				<< QVector3D(0, 0, 0) << length * QVector3D(0, 1, 0)
				<< QVector3D(0, 0, 0) << length * QVector3D(0, 0, 1);
			QVector<QVector4D> Cs; Cs
				<< QVector4D(1, 0, 0, 1) << QVector4D(1, 0, 0, 1) //r
				<< QVector4D(0, 1, 0, 1) << QVector4D(0, 1, 0, 1) //g
				<< QVector4D(0, 0, 1, 1) << QVector4D(0, 0, 1, 1);//b
			_axesRenderer.reset(new ShaderRenderer(Vs, Cs));
			_axesRenderer->setMode(GL_LINES);
		}
		_axesRenderer->setLineWidth(3.0);
		_axesRenderer->renderPerVertexColor(_cameraMatrix);
	}
}

void MainToolView::drawGround()
{
	if (_showGround)
	{
		if (!_groundRenderer)
		{
			// Build grid geometry
			auto ps = ParSingleton::instance();
            double gridSpacing = 1;
			double gridWidth = 10;  double gh = 0;
			QVector<QVector3D> Vs; Vs 
				<< QVector3D(0, gh, gridWidth) << QVector3D(0, gh, -gridWidth)   // x = 0
				<< QVector3D(gridWidth, gh, 0) << QVector3D(-gridWidth, gh, 0);  // z = 0
			for (GLfloat i = gridSpacing; i <= gridWidth; i += gridSpacing)	{ Vs 
				<< QVector3D(i, gh, gridWidth) << QVector3D(i, gh, -gridWidth)   // x =  i
				<< QVector3D(-i, gh, gridWidth) << QVector3D(-i, gh, -gridWidth)   // x = -i
				<< QVector3D(gridWidth, gh, i) << QVector3D(-gridWidth, gh, i)   // z =  i
				<< QVector3D(gridWidth, gh, -i) << QVector3D(-gridWidth, gh, -i);  // z = -i
			}
			_groundRenderer.reset(new ShaderRenderer(Vs));
			_groundRenderer->setMode(GL_LINES);
		}
		_groundRenderer->renderUniformColorSphereFading(QColor::fromRgbF(0.5, 0.5, 0.5, 0.1), _cameraMatrix);
	}
}

void MainToolView::drawMesh()
{
	if (_showMesh && _meshModel)
	{
		if (!_meshRenderer)
		{
			QVector<QVector3D> Vs, Ns; _meshModel->getCompiledData(Vs, Ns);
			_meshRenderer.reset(new ShaderRenderer(Vs, Ns));
			_meshRenderer->setMode(GL_TRIANGLE_WIREFRAME__);
		}
		_meshRenderer->renderUniformColorLighting(ParSingleton::instance()->colorMesh, _meshModel->getTransform(), _cameraMatrix, _cameraPos);
	}
}

void MainToolView::drawPoints()
{
	// points
	if (_showPoints && !_points.isEmpty())
	{
		if (!_pointsRenderer){
			_pointsRenderer.reset(new ShaderRenderer(_points));
			_pointsRenderer->setMode(_pointsMode);
			_pointsRenderer->setPointSize(5.0);
			_pointsRenderer->setLineWidth(2.0);
		}
		_pointsRenderer->renderUniformColor(Qt::red, _cameraMatrix);
	}

	// points2
	if (_showPoints && !_points2.isEmpty())
	{
		if (!_points2Renderer) 	{
			_points2Renderer.reset(new ShaderRenderer(_points2));
			_points2Renderer->setMode(_points2Mode);
			_points2Renderer->setPointSize(5.0);
			_points2Renderer->setLineWidth(2.0);
		}
		_points2Renderer->renderUniformColor(Qt::green, _cameraMatrix);
	}
}

void MainToolView::drawPlanes()
{
	auto ps = ParSingleton::instance();
    float scale = 5;

	if (_showPlanes && !_planes.isEmpty())
	{
		if (!_planesRenderer)
		{
			QVector<QVector3D> Vs;
			for (auto p : _planes) Vs << getCompiledTriangles(p, scale);
			_planesRenderer.reset(new ShaderRenderer(Vs));
			_planesRenderer->setMode(GL_TRIANGLES);
		}
		_planesRenderer->renderUniformColor(QColor(237, 125, 49, 160), _cameraMatrix);

		if (!_planesOrientRenderer)
		{
			QVector<QVector3D> Vs;
			for (auto p : _planes) Vs << p.Constant << p.Constant + scale * p.Normal;
			_planesOrientRenderer.reset(new ShaderRenderer(Vs));
			_planesOrientRenderer->setMode(GL_LINES);
			_planesOrientRenderer->setLineWidth(3.0);
		}
		_planesOrientRenderer->renderUniformColor(Qt::red, _cameraMatrix);
	}
}

void MainToolView::drawDebug()
{
}

void MainToolView::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    _mouseMoveCursorPos = event->pos();

    if(_rightButtonDown)
    {
        _trackball->track(Eigen::Vector2i(_mouseMoveCursorPos.x(), _mouseMoveCursorPos.y()));
    }

	if (_middleButtonDown)
	{
		auto delta = _mouseMoveCursorPos - _buttonDownCursorPos;

        double t = 0.005;
        Eigen::Vector3f up = - _camera->up() * t * delta.y();
        Eigen::Vector3f right = _camera->right() * t * delta.x();
		Eigen::Vector3f d = up + right;

		_camera->setPosition(_cameraLoc - d);
	}

    scene()->update(rect);
}

void MainToolView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // Record mouse states
    _buttonDownCursorPos = event->pos();
    _mouseMoveCursorPos = _buttonDownCursorPos;
    _leftButtonDown = event->buttons() & Qt::LeftButton;
    _rightButtonDown = event->buttons() & Qt::RightButton;
    _middleButtonDown = event->buttons() & Qt::MiddleButton;

    // Camera movement
    if(_rightButtonDown)
    {
        _trackball->start(Eigen::Trackball::Around);
        _trackball->track(Eigen::Vector2i(_buttonDownCursorPos.x(), _buttonDownCursorPos.y()));
    }

	if (_middleButtonDown)
	{
		_cameraLoc = _camera->position();
	}
}

void MainToolView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	Q_UNUSED(event);
}

void MainToolView::wheelEvent(QGraphicsSceneWheelEvent * event)
{
	QGraphicsObject::wheelEvent(event);
	double step = (double)event->delta() / 1200;
	_camera->zoom(step);

	scene()->update(rect);
}


void MainToolView::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{

	case Qt::Key_A:
	{
		_showAxes = !_showAxes;
		break;
	}
	case Qt::Key_C:
	{
		break;
	}
	case  Qt::Key_D:
	{
		_showDebugMesh = !_showDebugMesh;
		break;
	}
	case Qt::Key_G:
	{
		if (event->modifiers() & Qt::ControlModifier)
		{
			_meshModel->snap2Ground();
		}
		else
		{
			_showGround = !_showGround;
		}
		break;
	}
	case Qt::Key_M:
	{
		_showMesh = !_showMesh;
		break;
	}
	case Qt::Key_O:
	{
		_showPlanes = !_showPlanes;
		break;
	}
	case Qt::Key_P:
	{
		_showPoints = !_showPoints;
		break;
	}

	case Qt::Key_W:
	{
		_showMeshWireframe = !_showMeshWireframe;
		if (_showMeshWireframe)
			_meshRenderer->setMode(GL_TRIANGLE_WIREFRAME__);
		else
			_meshRenderer->setMode(GL_TRIANGLES);

		break;
	}
	case Qt::Key_X:
	{
		if (_meshModel)
		{
			_meshModel->rotate(90, 1, 0, 0);
		}
		break;
	}
	case Qt::Key_Y:
	{
		if (_meshModel)
		{
			_meshModel->rotate(90, 0, 1, 0);
		}
		break;
	}
	case Qt::Key_Z:
	{
		if (_meshModel)
		{
			_meshModel->rotate(90, 0, 0, 1);
		}
		break;
	}
	default:
		break;
	}

	scene()->update(rect);
}


void MainToolView::setMesh(MeshModel::Ptr mesh)
{
	_meshModel = mesh;

	_meshRenderer.reset(nullptr);
}

void MainToolView::setPoints(QVector<QVector3D>& pnts, GLenum mode)
{
	_points = pnts;
	_pointsMode = mode;

	_pointsRenderer.reset(nullptr);
}

void MainToolView::setPoints2(QVector<QVector3D>& pnts, GLenum mode)
{
	_points2 = pnts;
	_points2Mode = mode;

	_points2Renderer.reset(nullptr);
}


void MainToolView::setPlanes(QVector<Plane> planes)
{
	_planes = planes;

	for (auto&& plane : _planes)
	{
		plane.Constant += 1e-3f * plane.Normal;
	}

	_planesRenderer.reset(nullptr);
	_planesOrientRenderer.reset(nullptr);
}

void MainToolView::resetGroundGrids()
{
	_groundRenderer.reset(nullptr);
}

QVector<QVector3D> MainToolView::getCompiledTriangles(Plane plane, float scale)
{
    // Compute two arbitrary orthogonal vectors
    auto orthogonalVector = [](const QVector3D& n) {
        if ((abs(n.y()) >= 0.9 * abs(n.x())) && abs(n.z()) >= 0.9 * abs(n.x())) return QVector3D(0.0, -n.z(), n.y());
        else if ( abs(n.x()) >= 0.9 * abs(n.y()) && abs(n.z()) >= 0.9 * abs(n.y()) ) return QVector3D(-n.z(), 0.0, n.x());
        else return QVector3D(-n.y(), n.x(), 0.0);
    };

	auto o = plane.Constant;
	auto n = plane.Normal;
    auto u = orthogonalVector(n);
    auto v = QVector3D::crossProduct(n, u);

	auto p0 = o + (-u + v) * scale;
	auto p1 = o + ( u + v) * scale;
	auto p2 = o + ( u - v) * scale;
	auto p3 = o + (-u - v) * scale;

	return QVector<QVector3D>()<< p0 << p1 << p2 << p0 << p2 << p3;
}
