#pragma once

#include <Eigen/Core>
#include <QVector3D>

inline bool areEqual(double a, double b)
{
	return (fabs(a - b) < 1e-6);
}

inline bool areParallel(QVector3D u, QVector3D v)
{
	u.normalize();
	v.normalize();
	auto d = fabs(QVector3D::dotProduct(u, v));
	return areEqual(d, 1);
}

inline bool areSameDirection(QVector3D u, QVector3D v)
{
	u.normalize();
	v.normalize();
	auto d = QVector3D::dotProduct(u, v);
	return areEqual(d, 1);
}

inline bool areOppositeDirection(QVector3D u, QVector3D v)
{
	u.normalize();
	v.normalize();
	auto d = QVector3D::dotProduct(u, v);
	return areEqual(d, -1);
}

inline Eigen::Vector3d eigenV(QVector3D& v)
{
	return Eigen::Vector3d(v.x(), v.y(), v.z());
}


inline QVector3D qtV(Eigen::Vector3d v)
{
	return QVector3D(v.x(), v.y(), v.z());
}

#define M_PI 3.14159265358979323846

inline double radians2degrees( double r )
{
    return 180.0 * r / M_PI;
}

inline double degrees2radians( double a )
{
    return M_PI * a / 180;
}

