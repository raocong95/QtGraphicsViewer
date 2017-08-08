#pragma once

#include <QVector>
#include <QVector3D>
#include <QVector4D>

class Line;

class Plane
{
public:
    typedef QVector3D Vector3;
    typedef QVector3D Point;
    typedef QVector4D Vector4;

    Plane();
	Plane(Vector3 c, Vector3 n);

	double	signedDistanceTo(Vector3 p);
	double	distanceTo(Vector3 p);

	int		whichSide(Vector3 p);
	bool	onSameSide( QVector<Vector3>& pnts );

	bool	contains(Line line);

	Vector3 getProjection(Vector3 p);
	Vector3 getIntersection(Line &line);

	void flip();
	Plane flipped();

	void translate(Vector3 t);
	Plane translated(Vector3 t);

	bool fitFromPoints(const std::vector<Point> &points);
	Vector4 getCoeff();

public:
	Vector3 Constant, Normal;
};
