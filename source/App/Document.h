#pragma once

#include <QObject>
#include <MeshModel.h>

class GraphicsScene;

class Document : public QObject
{
    Q_OBJECT

public:
    Document(GraphicsScene* scene);

	// mesh
	void loadMesh(QString filename);
	void saveMesh(QString filename);

public:
	GraphicsScene* _scene;

	MeshModel::Ptr _mesh;
};
