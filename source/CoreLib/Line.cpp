#include "Line.h"
#include "CoreLibUtility.h"

Line::Line()
{
	Origin = Vector3(0, 0, 0);
	Direction = Vector3(1, 0, 0);
}

Line::Line( Vector3 o, Vector3 d )
	:Origin(o), Direction(d)
{
	Direction.normalize();
}

Line::Vector3 Line::getPoint( double t )
{
	return Origin + t * Direction;
}

bool Line::contains( Vector3 p )
{
	Vector3 O2p = p - Origin;
	if (areEqual(O2p.length(), 0))
	{
		return true;
	}
	else
	{
		return areParallel(Direction, O2p);
	}
}

double Line::getProjTime( Vector3 p )
{
	Vector3 o2p = p - Origin;
	return Vector3::dotProduct(Direction, o2p);
    //return Direction.dot(o2p);
}

