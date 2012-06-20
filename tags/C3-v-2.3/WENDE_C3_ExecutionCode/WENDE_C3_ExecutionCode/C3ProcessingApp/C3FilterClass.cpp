#include "StdAfx.h"
#include "C3FilterClass.h"
#include <math.h>
#include <iostream>
#include "C3Configuration.h"

using std::endl;
using std::cout;

//#define DEBUG_PRINT
C3FilterClass::C3FilterClass(void) :
			   m_isInit(false)
{


	this->processNoise = C3Configuration::Instance().PROCESS_NOISE;
	I = MatrixXd::Identity(6,6);
	F = MatrixXd::Identity(6,6);
	H = MatrixXd::Identity(2,6);
	P = MatrixXd::Ones(6,6)*processNoise;
	X = MatrixXd::Zero(6,1);
	R = MatrixXd::Identity(2,2)*processNoise;
	#ifdef DEBUG_PRINT
	cout << "***BEGIN INITILIZATION***" << endl;
	cout << "Process Noise = " << C3ProcessingConfiguration::Instance().PROCESS_NOISE << endl;
	cout << "--I Matrix--\n" << I << endl;
	cout << "--P Matrix--\n" << P << endl;
	cout << "--X Matrix--\n" << X << endl;
	cout << "--R Matrix--\n" << R << endl;
	cout << "--H Matrix--\n" << H << endl;
	cout << "***END INITILIZATION***"  << endl << endl;
	#endif
}
C3FilterClass::C3FilterClass(const C3FilterClass &rhs)
{
	*this = rhs;
}
C3FilterClass & C3FilterClass::operator= (const C3FilterClass &rhs)
{
	// Make sure that they are not the same
	if (this != &rhs)
	{
		// TODO ...
	}
	// by convention, always return *this
	return *this;
}
C3FilterClass::~C3FilterClass(void)
{

}

C3_TRACK_POINT_DOUBLE C3FilterClass::FilterInput(C3_TRACK_POINT_DOUBLE cameraRoverPositions, double updateTime)	
{
	static int processingIteration = 0;
	Matrix<double, 1, 2> crPos;
	crPos(0,0) = cameraRoverPositions.X;
	crPos(0,1) = cameraRoverPositions.Y;

	// If we have not Initilized the filter then init the filter state
	if (this->m_isInit == false)
	{
		X(0,0) = crPos(0,0);
		X(1,0) = crPos(0,1);
		this->m_isInit  = true;
	}

	// setup the F matrix
	for (int ii = 0; ii < F.cols()-2; ii++)
	{
		F(ii,ii+2) = updateTime; 
	}
	
	// prediction
	X = F * X;
	P = F * P * F.transpose() + I;
	S = H * P.transpose() * H.transpose() + R;
	B = H * P.transpose();
	p(0,0) = sqrt(P(0,0));	
	p(0,1) = sqrt(P(1,1));

	#ifdef DEBUG_PRINT
	cout << "***PREDICTION PROCESSING (" << ++processingIteration << ")***" << endl;
	cout << "--X Matrix--\n" << X << endl;
	cout << "--F Matrix--\n" << F << endl;
	cout << "--S Matrix--\n" << S << endl;
	cout << "--B Matrix--\n" << B << endl;
	cout << "--p Matrix--\n" << p << endl;
	cout << endl;
	#endif
	// save the residuals
	rk(0,0) = crPos(0,0) - X(0,0);
	rk(0,1) = crPos(0,1) - X(1,0);
	#ifdef DEBUG_PRINT
	cout << "***Residuals PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--X Matrix--\n" << rk << endl;
	cout << endl;
	#endif

	// Determine the Gain Matrix
	K = (S.inverse()*B).transpose();
	#ifdef DEBUG_PRINT
	cout << "***Gain PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--K Matrix--\n" << K << endl;
	cout << endl;
	#endif

	// Esitimated state and covariance
	X = X + K * (crPos.transpose() - H * X);
	P = P - K * H * P;
	#ifdef DEBUG_PRINT
	cout << "***Estimate State and Cov PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--P Matrix--\n" << P << endl;
	cout << "--X Matrix--\n" << X << endl;
	cout << endl;
	#endif

	// Compute the estimated measurement
	y = H * X;
	xk(0,0) = X(0,0);
	xk(0,1) = X(1,0);
	vk(0,0) = X(2,0);
	vk(0,1) = X(3,0);
	#ifdef DEBUG_PRINT
	cout << "***Estimate Measurement PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--y Matrix--\n" << y << endl;
	cout << "--xk Matrix--\n" << xk << endl;
	cout << "--vk Matrix--\n" << vk << endl;
	cout << endl;
	#endif

	// Calculate Predicted Intercept Point PIP
	// return the camera point
	return  GetPredictedPoint(updateTime);
}
C3_TRACK_POINT_DOUBLE C3FilterClass::GetPredictedPoint(double time)
{
	// Calculate Predicted Intercept Point PIP
	C3_TRACK_POINT_DOUBLE cameraPoint;
	cameraPoint.X = xk(0,0)+ vk(0,0) * time;
	cameraPoint.Y = xk(0,1)+ vk(0,1) * time;
	return cameraPoint;
}

