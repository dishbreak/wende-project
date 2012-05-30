#pragma once
#include <vector>
#include "Dense"
#include "ShmStructs.h"
using namespace Eigen;
using std::vector;

typedef struct {
} AlphaBetaState;

typedef struct {
	Matrix<double, 6, 1> X;
	Matrix<double, 6, 6> P;
	Matrix<double, 6, 6> I;
	Matrix<double, 2, 2> R;
	Matrix<double, 6, 6> F;
	Matrix<double, 2, 6> H;
	Matrix<double, 2, 2> S;
	Matrix<double, 2, 6> B;
	Matrix<double, 6, 2> K;
	Matrix<double, 2, 1> y;
	Matrix<double, 1, 2> rk;
	Matrix<double, 1, 2> vk;
	Matrix<double, 1, 2> xk;
	Matrix<double, 1, 2> p;
	double processNoise;
} KalmanVariables;

class C3FilterClass
{
	// Filter Internal variables
	private:
		bool			m_isInit;
		KalmanVariables m_kalman;

	// Filter Cononical Functions
	public:
		C3FilterClass(void);
		C3FilterClass(const C3FilterClass &rhs);
		C3FilterClass & operator= (const C3FilterClass &rhs);
		~C3FilterClass(void);

	// Public Functions
	public:
		C3_TRACK_POINT_DOUBLE FilterInput(C3_TRACK_POINT_DOUBLE cameraRoverPositions, double updateTime);	

	// Private Functions
	private:
		
};
