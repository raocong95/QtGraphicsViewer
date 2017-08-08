#include "MeshModel.h"

#include <qglobal.h>
#include <QTime>
#include "CoreLibUtility.h"

using namespace opengp::SurfaceMesh;

MeshModel::MeshModel(QString path /*= QString()*/, QString name /*= QString()*/)
: SurfaceMeshModel(path, name), TransfModel()
{
}


MeshModel::~MeshModel()
{
}

MeshModel::Ptr MeshModel::combineMeshes(Ptr m0, Ptr m1)
{
	MeshModel::Ptr mesh = QSharedPointer<MeshModel>(new MeshModel());
	auto vertices0 = m0->vertex_coordinates();
	for (auto v : m0->vertices()) {
		mesh->add_vertex(vertices0[v]);
	}
	for (auto f : m0->faces()) {
		std::vector<opengp::SurfaceMesh::Vertex> v_indices;
		for (auto vf : m0->vertices(f)) {
			v_indices.push_back(opengp::SurfaceMesh::Vertex(vf.idx()));
		}
		mesh->add_face(v_indices);
	}

	auto vertices1 = m1->vertex_coordinates();
	for (auto v1 : m1->vertices()) {
		mesh->add_vertex(vertices1[v1]);
	}
	for (auto f1 : m1->faces()) {
		std::vector<opengp::SurfaceMesh::Vertex> v_indices;
		for (auto vf : m1->vertices(f1)) {
			v_indices.push_back(opengp::SurfaceMesh::Vertex(vf.idx() + m0->n_vertices()));
		}
		mesh->add_face(v_indices);
	}
	return mesh;
}

void MeshModel::computeAABB()
{
	updateBoundingBox();
	m_aabb = _bbox;
}

void MeshModel::applyTransform()
{
	// apply the transformation
	auto points = vertex_coordinates();
	for (auto v : vertices())
	{
		QVector3D q = m_tranform * qtV(points[v]);
		points[v] = eigenV(q);
	}

	// reset the transformation
	m_tranform.setToIdentity();
}


void MeshModel::scaleAroundCenter(double scale)
{
	//compute center
	auto vertices = vertex_coordinates();
	opengp::Vec3 total_p(0,0,0);
	for (auto v : this->vertices())
	{
		auto p = vertices[v];
		total_p += p;
	}
	auto center = total_p / this->n_vertices();

	for (auto v : this->vertices())
	{
		vertices[v] = (vertices[v] - center) * scale;
		vertices[v] += center;
	}	
}

QVector3D MeshModel::getFaceCenter(uint fid)
{
	Vector3 c(0, 0, 0);
	int n = 0;
	auto points = vertex_coordinates();
	for (auto vf : vertices(Face(fid)))
	{
		c += points[vf];
		n++;
	}
	if(n>0) c /= n;

	return qtV(c);
}

QVector<QVector3D> MeshModel::getFacePoints(uint fid)
{
	QVector<QVector3D> fps;
	auto points = vertex_coordinates();
	for (auto vf : vertices(Face(fid)))
	{
		fps << qtV(points[vf]);
	}

	return fps;
}

double MeshModel::getSurfaceArea()
{
	double A = 0;
	auto points = vertex_coordinates();
	for (auto f : faces())
	{
		QVector<Vector3> fps;
		for (auto vf : vertices(f)) { fps << points[vf]; }
		auto area = 0.5 * (fps[1] - fps[0]).cross(fps[2] - fps[0]).norm();
		A += area;
	}

	return A;
}

void MeshModel::getCompiledData(QVector<QVector3D>& Vs, QVector<QVector3D>& Ns)
{
	Vs = getCompiledVertices();
	Ns = getCompiledVertexNormals();
}

QVector<QVector3D> MeshModel::getCompiledVertices()
{
	QVector<QVector3D> Vs;
	auto mesh_points = vertex_coordinates();
	for (auto f : faces()) {
		for (auto vf : vertices(f)) {
			Vector3 v = mesh_points[vf];
			Vs << qtV(v);
		}
	}

	return Vs;
}

QVector<QVector3D> MeshModel::getCompiledVertexNormals()
{
	QVector<QVector3D> Ns;

	auto mesh_normals_v = vertex_normals();
	for (auto f : faces()) {
		for (auto vf : vertices(f)) {
			Vector3 n = mesh_normals_v[vf];
			Ns << qtV(n);
		}
	}

	return Ns;
}

QVector<QVector3D> MeshModel::getCompiledFaceNormals()
{
	QVector<QVector3D> Ns;

	auto mesh_normals_f = face_normals();
	for (auto f : faces()) {
		for (auto vf : vertices(f)) {
			Vector3 n = mesh_normals_f[f];
			Ns << qtV(n);
		}
	}

	return Ns;
}

QVector<MeshModel::SamplePoint> MeshModel::sampleSurfaceByDensity(double density)
{
	QVector<SamplePoint> samples;

	// prepare
	qsrand(QTime().msecsSinceStartOfDay());
	auto points = vertex_coordinates();
	auto fnormals = face_normals();

	// sample face by face
	for (auto f : faces())
	{
		// face points and area
		QVector<Vector3> fps;
		for (auto vf : vertices(f))	{fps << points[vf];}
		auto area = 0.5 * (fps[1] - fps[0]).cross(fps[2] - fps[0]).norm();

		// number of samples
		double D = area * density; int nSamples = (int)floor(D);
		double t = (double)qrand() / RAND_MAX;
		if (D - floor(D) > t) nSamples++;

		// sample
		for (int i = 0; i < nSamples; i++)
		{
			SamplePoint sp;

			double alpha = (double)qrand() / RAND_MAX;
			double beta = (double)qrand() / RAND_MAX;
			double sBeta = sqrt(beta);
			sp.position = sBeta * ((1 - alpha) * fps[0] + alpha * fps[1])
				+ (1 - sBeta) * fps[2];
			sp.normal = fnormals[f];

			samples << sp;
		}
	}

	return samples;
}

bool MeshModel::hasVertexColor()
{
	return has_vertex_property<Vector3>("v:color") || has_vertex_property<Vector3>("v:colori");;
}

void MeshModel::extractSaliencyRegions()
{
	auto integer_colors = vertex_property<Eigen::Vector3i>("v:colori");
	map<Eigen::Vector3i, QVector<QVector3D>, IntegerColorComp> color_vertex_map;
	auto vertices_here = vertex_coordinates();
	for (auto v : vertices()) {
		QVector3D v_here(QVector3D(vertices_here[v].x(), vertices_here[v].y(), vertices_here[v].z()));
		auto iter_here = color_vertex_map.find(integer_colors[v]);
		if (iter_here != color_vertex_map.end()) {
			//encountered already
			iter_here->second.push_back(v_here);
		}
		else
		{
			QVector<QVector3D> array_here;
			array_here.push_back(v_here);
			color_vertex_map.insert(pair<Eigen::Vector3i, QVector<QVector3D>>(integer_colors[v], array_here));
		}
	}

	_saliencyRegions.clear();

	for (auto iter = color_vertex_map.begin(); iter != color_vertex_map.end(); iter++) {
		if (iter->first.x() == 150 && iter->first.y() == 150 && iter->first.z() == 150) {
			//default color, ignore
		}
		else {
			if(iter->second.size()>10) //filter out noise
				_saliencyRegions.push_back(iter->second);
		}
	}

}

QVector<MeshModel::SamplePoint> MeshModel::sampleSurfaceByNumber(int N)
{
	double A = getSurfaceArea();
	return sampleSurfaceByDensity(N / A);
}
