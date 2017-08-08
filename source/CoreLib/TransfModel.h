#pragma once

#include <QMatrix4x4>
#include <Eigen/Core>
#include <Eigen/Geometry>

class TransfModel
{
public:
    TransfModel();

public:
	void translate(const QVector3D & vector);
	void translate(const Eigen::Vector3d & vector);
	void translate(float x, float y, float z);
	void rotate(float angle, const QVector3D & vector);
	void rotate(float angle, float x, float y, float z);
	void rotate(const QQuaternion & quaternion);
	void scale(const QVector3D & vector);
	void scale(float x, float y, float z);
	void scale(float factor);

	virtual void computeAABB() = 0;
	Eigen::AlignedBox3d getAABB();
	void snap2Ground();
	void alignCenter2Origin();

	void setTransform(const QMatrix4x4 & transform);
	QMatrix4x4 getTransform();

	// NOTE: the model data will not be modified until this function is applied
	virtual void applyTransform() = 0;

protected:
	QMatrix4x4	m_tranform;
	Eigen::AlignedBox3d m_aabb;
};

