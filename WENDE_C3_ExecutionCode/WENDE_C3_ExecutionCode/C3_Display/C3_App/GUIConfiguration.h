///////////////////////////////////////////////////////////////////////////////////
//		 File: CGUIConfiguration.h
// Programmer: Benjamin Kurt Heiner
//       Date: May 22 2012
//
//    Purpose: This class is the single holder of GUI Configuration parameters
///////////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////
// Include Directives
/////////////////////////////////////////////////////////////
#include <string>
using std::string;

class CGUIConfiguration
{
/////////////////////////////////////////////////////////////
// Public Functions... singleton items
/////////////////////////////////////////////////////////////
public:
	//Singleton Manager for the shatred memory
	static CGUIConfiguration& Instance()
	{
		// create singleton
		static CGUIConfiguration singleton;
		// return reference to singleton
		return singleton;
	}
	// destructor
	~CGUIConfiguration(void);								
/////////////////////////////////////////////////////////////
// Public Functions...
/////////////////////////////////////////////////////////////
public:
	void WriteXMLFile();
	void ReadXMLFile();

/////////////////////////////////////////////////////////////
// Public Variables...
/////////////////////////////////////////////////////////////
public:
	//Configuration file
	string CfgFile;								// The configuration file!!!
	//Laser status messages
	string SHM_C3_LASER_STATUS;					// Laser STATUS SHM File Name
	string SHM_C3_LASER_STATUS_MUTEX;			// Laser STATUS MUTEX File Name
	string SHM_C3_LASER_STATUS_EVENT1;			// Laser STATUS Event1 File Name
	string SHM_C3_LASER_STATUS_EVENT2;			// Laser STATUS Event2 File Name
	//Laser SHM Files
	string SHM_C3_LASER_POINTING;				// Laser POINTING SHM File Name
	string SHM_C3_LASER_POINTING_MUTEX;			// Laser POINTING MUTEX File Name
	string SHM_C3_LASER_POINTING_EVENT1;		// Laser POINTING Event1 File Name
	string SHM_C3_LASER_POINTING_EVENT2;		// Laser POINTING Event2 File Name
	//Camera status messages
	string SHM_C3_CAMERA_STATUS;				// Camera Status SHM File Name
	string SHM_C3_CAMERA_STATUS_MUTEX;			// Camera Status MUTEX File Name
	string SHM_C3_CAMERA_STATUS_EVENT1;			// Camera Status Event1 File Name
	string SHM_C3_CAMERA_STATUS_EVENT2;			// Camera Status Event2 File Name
	//Camera SHM Files
	string SHM_C3_CAMERA_TRACK;					// Camera Track SHM File Name
	string SHM_C3_CAMERA_TRACK_MUTEX;			// Camera Track MUTEX File Name
	string SHM_C3_CAMERA_TRACK_EVENT1;			// Camera Track Event1 File Name
	string SHM_C3_CAMERA_TRACK_EVENT2;			// Camera Track Event2 File Name
	//Camera image
	string SHM_C3_CAMERA_IMAGE;					// Camera IMAGE SHM File Name
	string SHM_C3_CAMERA_IMAGE_MUTEX;			// Camera IMAGE MUTEX File Name
	string SHM_C3_CAMERA_IMAGE_EVENT1;			// Camera IMAGE Event1 File Name
	string SHM_C3_CAMERA_IMAGE_EVENT2;			// Camera IMAGE Event2 File Name
	//Process interface SHM
	string SHM_C3_PROCESSING_STATUS;			// Processing IMAGE SHM File Name
	string SHM_C3_PROCESSING_STATUS_MUTEX;		// Processing IMAGE MUTEX File Name
	string SHM_C3_PROCESSING_STATUS_EVENT1;		// Processing IMAGE Event1 File Name
	string SHM_C3_PROCESSING_STATUS_EVENT2;		// Processing IMAGE Event2 File Name
	// DEBUG ITEMS
	bool isShowDebugPannel;						// Flag to determine if we show
												// the debug pannel
/////////////////////////////////////////////////////////////
// Private Functions... singleton items
/////////////////////////////////////////////////////////////
private:
	// Private constructor
	CGUIConfiguration();								
	// Prevent copy-construction
	CGUIConfiguration(const CGUIConfiguration&);           
	// Prevent assignment
	CGUIConfiguration& operator=(const CGUIConfiguration&);

};