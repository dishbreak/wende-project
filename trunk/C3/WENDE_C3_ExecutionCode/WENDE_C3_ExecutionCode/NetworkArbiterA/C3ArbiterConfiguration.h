#pragma once
#include <string>
#include "ConnectionSettings.h"
using std::string;

class C3ArbiterConfiguration
{

public:
	C3ArbiterConfiguration(void);
	~C3ArbiterConfiguration(void);
	void WriteXMLFile();
	void ReadXMLFile();

public:
	//Configuration file
	string CfgFile;								// The configuration file!!!
	//Laser status messages
	string SHM_C3_LASER_STATUS;
	string SHM_C3_LASER_STATUS_MUTEX;
	string SHM_C3_LASER_STATUS_EVENT1;
	string SHM_C3_LASER_STATUS_EVENT2;
	//Laser SHM Files
	string SHM_C3_LASER_POINTING;					// Laser SHM File Name
	string SHM_C3_LASER_POINTING_MUTEX;			// Laser MUTEX File Name
	string SHM_C3_LASER_POINTING_EVENT1;			// Laser Event1 File Name
	string SHM_C3_LASER_POINTING_EVENT2;			// Laser Event2 File Name
	//Camera status messages
	string SHM_C3_CAMERA_STATUS;
	string SHM_C3_CAMERA_STATUS_MUTEX;
	string SHM_C3_CAMERA_STATUS_EVENT1;
	string SHM_C3_CAMERA_STATUS_EVENT2;
	//Camera SHM Files
	string SHM_C3_CAMERA_TRACK;					// Camera SHM File Name
	string SHM_C3_CAMERA_TRACK_MUTEX;				// Camera MUTEX File Name
	string SHM_C3_CAMERA_TRACK_EVENT1;			// Camera Event1 File Name
	string SHM_C3_CAMERA_TRACK_EVENT2;			// Camera Event2 File Name
	//Camera image
	string SHM_C3_CAMERA_IMAGE;
	string SHM_C3_CAMERA_IMAGE_MUTEX;
	string SHM_C3_CAMERA_IMAGE_EVENT1;
	string SHM_C3_CAMERA_IMAGE_EVENT2;
	//
	ConnectionSettings Connection;
};
