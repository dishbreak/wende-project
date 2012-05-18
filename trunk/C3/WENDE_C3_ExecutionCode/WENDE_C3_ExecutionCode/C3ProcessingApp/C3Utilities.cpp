#include "StdAfx.h"
#include "C3Utilities.h"
#include <math.h>

double C3Utilities::EuclideanDistance(C3Point p1, C3Point p2)
{
	double A = (p1.X-p2.X);
	double B = (p1.Y-p2.Y);
	return sqrt(A*A+B*B);
}
double C3Utilities::EuclideanDistance(C3Point p1)
{
	double A = (p1.X-0.0);
	double B = (p1.Y-0.0);
	return sqrt(A*A+B*B);
}
