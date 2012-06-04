#include "stdafx.h"
#ifndef CALIBRATOR_H
#define CALIBRATOR_H

class Calibrator
{
public:
	Calibrator();
	~Calibrator();
	void InitCalibration();
	int HandleCalibrationResponse();

private:
	int SimProcAppResponse();
};
#endif //CALIBRATOR_H