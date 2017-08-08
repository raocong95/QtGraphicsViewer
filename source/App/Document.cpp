#include "Document.h"
#include "ParSingleton.h"
#include "GraphicsScene.h"

#include <QFileInfo>

Document::Document(GraphicsScene* scene)
	:_scene(scene)
{
}

void Document::loadMesh(QString filename)
{
	QFileInfo fileinfo(filename);

	auto ps = ParSingleton::instance();
	ps->dataPath = fileinfo.path();
	ps->meshFilename = filename;
	ps->meshBaseName = fileinfo.baseName();

	_mesh = MeshModel::Ptr(new MeshModel(ps->dataPath, fileinfo.baseName()));
	opengp::read_mesh(*_mesh, filename.toStdString());
	if (_mesh->hasVertexColor()) {
		_mesh->extractSaliencyRegions();
	}

	_mesh->update_face_normals();
	_mesh->update_vertex_normals();
}


void Document::saveMesh(QString filename)
{
	opengp::write_mesh(*_mesh, filename.toStdString());
}
