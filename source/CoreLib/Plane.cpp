#include "Plane.h"
#include "Line.h"

#include <Eigen/Eigen>

Plane::Plane()
{
	Constant = Vector3(0,0,0);
	Normal = Vector3(0,0,1);
}

Plane::Plane( Vector3 c, Vector3 n )
	:Constant(c), Normal(n)
{
	Normal.normalize();
}

double Plane::signedDistanceTo( Vector3 p )
{
	Vector3 dv = p - Constant;
    return QVector3D::dotProduct(dv, Normal);
}

double Plane::distanceTo( Vector3 p )
{
	return fabs(signedDistanceTo(p));
}

int Plane::whichSide(Vector3 p)
{
	double sd = signedDistanceTo(p);
    if (sd > 1e-6)
		return 1;
    else if(sd < -1e-6)
		return -1;
	else 
		return 0;
}

bool Plane::onSameSide( QVector<Vector3>& pnts )
{
	QVector<int> sides;
	for (Vector3 p : pnts) sides.push_back(whichSide(p));

	bool areSame = true;
	for (int i = 0; i < sides.size()-1; i++)
	{
		if (sides[i] != sides[i+1])
		{
			areSame = false;
			break;
		}
	}

	return areSame;
}

bool Plane::contains( Line line )
{
	return this->whichSide(line.getPoint(0)) == 0 
		&& this->whichSide(line.getPoint(1)) == 0;
}

Plane::Vector3 Plane::getProjection( Vector3 p )
{
	double dis = signedDistanceTo(p);
	return p - dis * Normal;
}

Plane::Vector3 Plane::getIntersection(Line &line)  
{
	double t = -distanceTo(line.Origin) / QVector3D::dotProduct(Normal, line.Direction);
	return line.Origin + line.Direction * t;
}

Plane Plane::flipped()
{
	return Plane(Constant, -Normal);
}

void Plane::translate( Vector3 t )
{
	Constant += t;
}

void Plane::flip()
{
	Normal *= -1;
}

Plane Plane::translated( Vector3 t )
{
	return Plane(Constant + t, Normal);
}

bool Plane::fitFromPoints(const std::vector<Point> &points)
{
    //for least square
    double coeffA = 0.0, coeffB = 0.0, coeffC = 0.0, coeffD = 0.0;
    int matrixSize = 3;
    Eigen::Matrix3Xd mpara(matrixSize, matrixSize);
    for (int i = 0; i < matrixSize; i++){
        for (int j = 0; j < matrixSize; j++){
            mpara(i, j) = 0;
        }
    }

    double sumx, sumy, sumz;
    double averx, avery, averz;

    sumx = sumy = sumz = 0;

    for (int i = 0; i < points.size(); i++){
        sumx += points[i].x();
        sumy += points[i].y();
        sumz += points[i].z();
    }

    averx = sumx / points.size();
    avery = sumy / points.size();
    averz = sumz / points.size();

    for (int i = 0; i < points.size(); i++){
        mpara(0, 0) += pow(points[i].x() - averx, 2);
        mpara(0, 1) += (points[i].x() - averx) * (points[i].y() - avery);
        mpara(0, 2) += (points[i].x() - averx) * (points[i].z() - averz);

        mpara(1, 0) += (points[i].x() - averx) * (points[i].y() - avery);
        mpara(1, 1) += pow(points[i].y() - avery, 2);
        mpara(1, 2) += (points[i].y() - avery) * (points[i].z() - averz);

        mpara(2, 0) += (points[i].x() - averx) * (points[i].z() - averz);
        mpara(2, 1) += (points[i].y() - avery) * (points[i].z() - averz);
        mpara(2, 2) += pow(points[i].z() - averz, 2);
    }

    Eigen::EigenSolver<Eigen::Matrix3Xd> msolver(mpara);
    std::complex<double> lambda1 = msolver.eigenvalues()[0];
    std::complex<double> lambda2 = msolver.eigenvalues()[1];
    std::complex<double> lambda3 = msolver.eigenvalues()[2];
    int minEigenValue = ((lambda1.real() < lambda2.real()) ? 0 : 1);
    minEigenValue = ((msolver.eigenvalues()[minEigenValue].real() < lambda3.real()) ? minEigenValue : 2);
    coeffA = msolver.eigenvectors().col(minEigenValue)[0].real();
    coeffB = msolver.eigenvectors().col(minEigenValue)[1].real();
    coeffC = msolver.eigenvectors().col(minEigenValue)[2].real();
    coeffD = -(coeffA * averx + coeffB * avery + coeffC * averz);

    Normal = Vector3(coeffA, coeffB, coeffC);
    Normal.normalize();

    float t = coeffD / (coeffA * coeffA + coeffB * coeffB + coeffC * coeffC);
    float x = -coeffA * t;
    float y = -coeffB * t;
    float z = -coeffC * t;
    Constant = Vector3(x, y, z);

	return true;
}

Plane::Vector4 Plane::getCoeff()
{
	double a_ = Normal.x();
	double b_ = Normal.y();
	double c_ = Normal.z();
	double d_ = -(a_*Constant.x() + b_*Constant.y() + c_*Constant.z());
	return Vector4(a_, b_, c_, d_);
}