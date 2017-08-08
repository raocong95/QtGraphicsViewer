#pragma once

#include <QVector3D>

class Line
{
public:
    typedef QVector3D Vector3;

    Line();
	Line(Vector3 o, Vector3 d);

	double getProjTime(Vector3 p);
	Vector3 getPoint(double t);

	bool	contains(Vector3 p);

public:
	Vector3 Origin, Direction;
};
