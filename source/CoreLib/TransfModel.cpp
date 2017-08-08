#include "TransfModel.h"
#include "CoreLibUtility.h"

TransfModel::TransfModel()
{
	m_tranform.setToIdentity();
	m_aabb.setEmpty();
}

QMatrix4x4 TransfModel::getTransform()
{
	return m_tranform;
}

Eigen::AlignedBox3d TransfModel::getAABB()
{
	return m_aabb;
}

void TransfModel::snap2Ground()
{
	computeAABB();
	auto min = qtV(m_aabb.min());
	auto tranMin = m_tranform * min;
	m_tranform.translate(0, -tranMin.y(), 0);
}

void TransfModel::alignCenter2Origin()
{
	computeAABB();
	auto center = qtV(m_aabb.center());
	auto tranCenter = m_tranform * center;
	m_tranform.translate(- tranCenter);
}

void TransfModel::setTransform(const QMatrix4x4 & transform)
{
	m_tranform = transform;
}

void TransfModel::scale(const QVector3D & vector)
{
	m_tranform.scale(vector);
}

void TransfModel::scale(float x, float y, float z)
{
	m_tranform.scale(x, y, z);
}

void TransfModel::scale(float factor)
{
	m_tranform.scale(factor);
}

void TransfModel::translate(const QVector3D & vector)
{
	m_tranform.translate(vector);
}

void TransfModel::translate(float x, float y, float z)
{
	m_tranform.translate(x, y, z);
}

void TransfModel::translate(const Eigen::Vector3d & vector)
{
	m_tranform.translate(vector.x(), vector.y(), vector.z());
}

void TransfModel::rotate(float angle, const QVector3D & vector)
{
	m_tranform.rotate(angle, vector);
}

void TransfModel::rotate(float angle, float x, float y, float z)
{
	m_tranform.rotate(angle, x, y, z);
}

void TransfModel::rotate(const QQuaternion & quaternion)
{
	m_tranform.rotate(quaternion);
}

