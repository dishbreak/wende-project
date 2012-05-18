#include "StdAfx.h"
#include "C3FilterClass.h"
#include <math.h>

C3FilterClass::C3FilterClass(void) :
			   m_isInit(false)
{
	// Setup the Kalman Filter State
	{
		this->m_kalman.processNoise = 9;

		for (int ii = 0; ii < 6; ii++)
		{
			m_kalman.X(ii) =  0;
			//Clear the P/I State for Kalman Filter State
			for (int jj = 0; jj < 6; jj++)
			{
				m_kalman.P(ii,jj) = 0;
				m_kalman.I(ii,jj) = (ii == jj)? 1 : 0;
			}
		}
		for (int ii = 0; ii < 2; ii++)
		{
			for (int jj = 0; jj < 2; jj++)
			{	
				m_kalman.R(ii,jj) = (ii == jj) ? m_kalman.processNoise : 0;
			}
		}		
	}
}
C3FilterClass::C3FilterClass(double processNoise) :
	m_isInit(false)
{
	// Setup the Kalman Filter State
	{
		this->m_kalman.processNoise = processNoise;

		for (int ii = 0; ii < 6; ii++)
		{
			m_kalman.X(ii) =  0;
			//Clear the P/I State for Kalman Filter State
			for (int jj = 0; jj < 6; jj++)
			{
				m_kalman.P(ii,jj) = 0;
				m_kalman.I(ii,jj) = (ii == jj)? 1 : 0;
				m_kalman.F(ii,jj) = (ii == jj)? 1 : 0;
			}
		}
		for (int ii = 0; ii < 2; ii++)
		{
			for (int jj = 0; jj < 2; jj++)
			{	
				m_kalman.R(ii,jj) = (ii == jj) ? m_kalman.processNoise : 0;
			}
			for (int jj = 0; jj < 6; jj++)
			{	
				m_kalman.H(ii,jj) = (ii == jj) ? 1 : 0;
			}
		}		
	}
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

C3Point C3FilterClass::FilterInput(C3Point cameraRoverPositions, double updateTime)	
{
	Matrix<double, 1, 2> crPos;
	crPos(0,0) = cameraRoverPositions.X;
	crPos(0,1) = cameraRoverPositions.Y;

	// initilize a camera 
	C3Point cameraPoint(0,0);		

	// If we have not Initilized the filter then init the filter state
	if (this->m_isInit == false)
	{
		m_kalman.X(0,0) = crPos(0,0);
		m_kalman.X(1,0) = crPos(0,1);
		this->m_isInit  = true;
	}

	// setup the F matrix
	for (int ii = 0; ii < 4; ii++)
	{
		m_kalman.F(ii,ii+2) = updateTime; 
	}

	// prediction
	m_kalman.X = m_kalman.F * m_kalman.X;
	m_kalman.P = m_kalman.F * m_kalman.P * m_kalman.F.transpose() + m_kalman.I;
	m_kalman.S = m_kalman.H * m_kalman.P.transpose() * m_kalman.H.transpose() + m_kalman.R;
	m_kalman.B = m_kalman.H * m_kalman.P.transpose();
	m_kalman.p00 = sqrt(m_kalman.P(0,0));	
	m_kalman.p11 = sqrt(m_kalman.P(1,1));

	// save the residuals
	m_kalman.rk(0,0) = crPos(0,0) - m_kalman.X(0,0);
	m_kalman.rk(0,1) = crPos(0,1) - m_kalman.X(1,0);

	// Determine the Gain Matrix
	// (S\B)' = (inv(S)*B)')
	m_kalman.K = (m_kalman.S.inverse()*m_kalman.B).transpose();

	// Esitimated state and covariance
	m_kalman.X = m_kalman.X + m_kalman.K * (crPos.transpose() - m_kalman.H * m_kalman.X);
	m_kalman.P = m_kalman.P - m_kalman.K * m_kalman.H * m_kalman.P;

	// Compute the estimated measurement
	m_kalman.y = m_kalman.H * m_kalman.X;
	m_kalman.xk(0,0) = m_kalman.X(0,0);
	m_kalman.xk(0,1) = m_kalman.X(1,0);
	m_kalman.vk(0,0) = m_kalman.X(2,0);
	m_kalman.vk(0,1) = m_kalman.X(3,0);

	// Calculate Predicted Intercept Point PIP
	cameraPoint.X = m_kalman.xk(0,0)+ m_kalman.vk(0,0) * updateTime;
	cameraPoint.Y = m_kalman.xk(0,1)+ m_kalman.vk(0,1) * updateTime;
	
	// return the camera point
	return cameraPoint;
}
