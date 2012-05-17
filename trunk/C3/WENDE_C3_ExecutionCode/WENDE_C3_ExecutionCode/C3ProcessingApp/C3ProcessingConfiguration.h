/////////////////////////////////////////////////////////////////////////////////
// File name: C3ProcessingConfiguration.h
// Creater: Benjamin Heiner
// Date: 5/17/2012
//
// purpose: this file contains all functions declerations for writing to and 
//			reading from the c3 processing configuration files
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

using std::string;

class C3ProcessingConfiguration
{
public:
	C3ProcessingConfiguration(void);				// Default Constructor
	~C3ProcessingConfiguration(void);				// Defualt Destructor
	void WriteXMLFile();							// Creates the initial configuration file
	void ReadXml();									// reads the configuration file

public:
	//Configuration file
	string m_CfgFile;								// The configuration file!!!
	//Camera SHM Files
	string m_SHM_C3_CAMERA_TRACK;					// Camera SHM File Name
	string m_SHM_C3_CAMERA_TRACK_MUTEX;				// Camera MUTEX File Name
	string m_SHM_C3_CAMERA_TRACK_EVENT1;			// Camera Event1 File Name
	string m_SHM_C3_CAMERA_TRACK_EVENT2;			// Camera Event2 File Name
	//Laser SHM Files
	string m_SHM_C3_LASER_POINTING;					// Laser SHM File Name
	string m_SHM_C3_LASER_POINTING_MUTEX;			// Laser MUTEX File Name
	string m_SHM_C3_LASER_POINTING_EVENT1;			// Laser Event1 File Name
	string m_SHM_C3_LASER_POINTING_EVENT2;			// Laser Event2 File Name
	//Kalman Filter Values
	int	   m_ProcessNoise;							// ...
};
