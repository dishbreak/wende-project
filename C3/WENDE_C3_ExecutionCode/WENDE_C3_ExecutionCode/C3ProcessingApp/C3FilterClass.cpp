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
	this->m_kalman.processNoise = C3Configuration::Instance().PROCESS_NOISE;
	m_kalman.I = MatrixXd::Identity(6,6);
	m_kalman.F = MatrixXd::Identity(6,6);
	m_kalman.H = MatrixXd::Identity(2,6);
	m_kalman.P = MatrixXd::Ones(6,6)*m_kalman.processNoise;
	m_kalman.X = MatrixXd::Zero(6,1);
	m_kalman.R = MatrixXd::Identity(2,2)*m_kalman.processNoise;
	#ifdef DEBUG_PRINT
	cout << "***BEGIN INITILIZATION***" << endl;
	cout << "Process Noise = " << C3ProcessingConfiguration::Instance().PROCESS_NOISE << endl;
	cout << "--I Matrix--\n" << m_kalman.I << endl;
	cout << "--P Matrix--\n" << m_kalman.P << endl;
	cout << "--X Matrix--\n" << m_kalman.X << endl;
	cout << "--R Matrix--\n" << m_kalman.R << endl;
	cout << "--H Matrix--\n" << m_kalman.H << endl;
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
		m_kalman.X(0,0) = crPos(0,0);
		m_kalman.X(1,0) = crPos(0,1);
		this->m_isInit  = true;
	}

	// setup the F matrix
	for (int ii = 0; ii < m_kalman.F.cols()-2; ii++)
	{
		m_kalman.F(ii,ii+2) = updateTime; 
	}
	
	// prediction
	m_kalman.X = m_kalman.F * m_kalman.X;
	m_kalman.P = m_kalman.F * m_kalman.P * m_kalman.F.transpose() + m_kalman.I;
	m_kalman.S = m_kalman.H * m_kalman.P.transpose() * m_kalman.H.transpose() + m_kalman.R;
	m_kalman.B = m_kalman.H * m_kalman.P.transpose();
	m_kalman.p(0,0) = sqrt(m_kalman.P(0,0));	
	m_kalman.p(0,1) = sqrt(m_kalman.P(1,1));

	#ifdef DEBUG_PRINT
	cout << "***PREDICTION PROCESSING (" << ++processingIteration << ")***" << endl;
	cout << "--X Matrix--\n" << m_kalman.X << endl;
	cout << "--F Matrix--\n" << m_kalman.F << endl;
	cout << "--S Matrix--\n" << m_kalman.S << endl;
	cout << "--B Matrix--\n" << m_kalman.B << endl;
	cout << "--p Matrix--\n" << m_kalman.p << endl;
	cout << endl;
	#endif
	// save the residuals
	m_kalman.rk(0,0) = crPos(0,0) - m_kalman.X(0,0);
	m_kalman.rk(0,1) = crPos(0,1) - m_kalman.X(1,0);
	#ifdef DEBUG_PRINT
	cout << "***Residuals PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--X Matrix--\n" << m_kalman.rk << endl;
	cout << endl;
	#endif

	// Determine the Gain Matrix
	m_kalman.K = (m_kalman.S.inverse()*m_kalman.B).transpose();
	#ifdef DEBUG_PRINT
	cout << "***Gain PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--K Matrix--\n" << m_kalman.K << endl;
	cout << endl;
	#endif

	// Esitimated state and covariance
	m_kalman.X = m_kalman.X + m_kalman.K * (crPos.transpose() - m_kalman.H * m_kalman.X);
	m_kalman.P = m_kalman.P - m_kalman.K * m_kalman.H * m_kalman.P;
	#ifdef DEBUG_PRINT
	cout << "***Estimate State and Cov PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--P Matrix--\n" << m_kalman.P << endl;
	cout << "--X Matrix--\n" << m_kalman.X << endl;
	cout << endl;
	#endif

	// Compute the estimated measurement
	m_kalman.y = m_kalman.H * m_kalman.X;
	m_kalman.xk(0,0) = m_kalman.X(0,0);
	m_kalman.xk(0,1) = m_kalman.X(1,0);
	m_kalman.vk(0,0) = m_kalman.X(2,0);
	m_kalman.vk(0,1) = m_kalman.X(3,0);
	#ifdef DEBUG_PRINT
	cout << "***Estimate Measurement PROCESSING (" << processingIteration << ")***" << endl;
	cout << "--y Matrix--\n" << m_kalman.y << endl;
	cout << "--xk Matrix--\n" << m_kalman.xk << endl;
	cout << "--vk Matrix--\n" << m_kalman.vk << endl;
	cout << endl;
	#endif

	// Calculate Predicted Intercept Point PIP
	// TODO --- DOES THIS CAST CAUSE A ISSUE????
	// return the camera point
	return  GetPredictedPoint(updateTime);
}
C3_TRACK_POINT_DOUBLE C3FilterClass::GetPredictedPoint(int time)
{
	// Calculate Predicted Intercept Point PIP
	// TODO --- DOES THIS CAST CAUSE A ISSUE????
	C3_TRACK_POINT_DOUBLE cameraPoint;
	cameraPoint.X = m_kalman.xk(0,0)+ m_kalman.vk(0,0) * time;
	cameraPoint.Y = m_kalman.xk(0,1)+ m_kalman.vk(0,1) * time;
	return cameraPoint;
}

