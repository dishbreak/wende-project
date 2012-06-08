#pragma once
#include <string>
#include "ConnectionSettings.h"
using std::string;

class C3Configuration
{
public:
	//Singleton Manager for the shatred memory
	static C3Configuration& Instance()
	{
		// create singleton
		static C3Configuration singleton;
		// return reference to singleton
		return singleton;
	}
	~C3Configuration(void);				// Defualt Destructor
	
	void WriteXMLFile();

private:
	void ReadXMLFile();
	// Private constructor
	C3Configuration();								
	// Prevent copy-construction
	C3Configuration(const C3Configuration&);           
	// Prevent assignment
	C3Configuration& operator=(const C3Configuration&);

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
	//Process interface SHM
	string SHM_C3_PROCESSING_STATUS;			// Processing IMAGE SHM File Name
	string SHM_C3_PROCESSING_STATUS_MUTEX;			// Processing IMAGE MUTEX File Name
	string SHM_C3_PROCESSING_STATUS_EVENT1;			// Processing IMAGE Event1 File Name
	string SHM_C3_PROCESSING_STATUS_EVENT2;			// Processing IMAGE Event2 File Name
	//network setup
	ConnectionSettings Connection;
	//WENDE SETUP
	double WENDE_PLAYING_FIELD_RADIUS;				// The radius of the playing field
	double WENDE_FAILURE_LINE_RADIUS;				// The Failure line for the WENDE POCs
	//Kalman Filter Values
	int	   PROCESS_NOISE;							// ...
	// DEBUG ITEMS
	bool isShowDebugPannel;							// Flag to determine if we show
};
