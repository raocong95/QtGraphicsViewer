#pragma once

#include <QColor>
#include <QMatrix4x4>
#include <QTextStream>

#include <map>
using namespace std;

#include "TransfModel.h"
#include "SurfaceMeshModel.h"
using opengp::SurfaceMesh::SurfaceMeshModel;
using opengp::SurfaceMesh::Vector3;

class MeshModel : public SurfaceMeshModel, public TransfModel
{
public:
	typedef QSharedPointer<MeshModel> Ptr;

	MeshModel(QString path = QString(), QString name = QString());
	MeshModel(const MeshModel&) = delete;
	~MeshModel();

	static MeshModel::Ptr combineMeshes(MeshModel::Ptr m0, MeshModel::Ptr m1);

	// transformable model
	void computeAABB() override;
	void applyTransform() override;
	void scaleAroundCenter(double scale);
	
	// querying geometry
	QVector3D getFaceCenter(uint fid);
	QVector<QVector3D> getFacePoints(uint fid);
	double getSurfaceArea();
	void getCompiledData(QVector<QVector3D>& Vs, QVector<QVector3D>& Ns);
	QVector<QVector3D> getCompiledVertices();
	QVector<QVector3D> getCompiledVertexNormals();
	QVector<QVector3D> getCompiledFaceNormals();

	// sampling
	struct SamplePoint {
		Vector3 position;
		Vector3 normal;
	};
	QVector<SamplePoint> sampleSurfaceByNumber(int N);
	QVector<SamplePoint> sampleSurfaceByDensity(double density);

	//for saliency regions
	struct IntegerColorComp {
		bool operator()(const Eigen::Vector3i &k1, const Eigen::Vector3i &k2)const
		{
			if (k1.x() != k2.x())
			{
				return k1.x() < k2.x();
			}

			if (k1.y() != k2.y())
			{
				return k1.y() < k2.y();
			}
			if (k1.z() != k2.z())
			{
				return k1.z() < k2.z();
			}
			return false;
		}
	};
	bool hasVertexColor();
	void extractSaliencyRegions();
	QVector<QVector<QVector3D>> _saliencyRegions; //each region is defined by a set of points

};
