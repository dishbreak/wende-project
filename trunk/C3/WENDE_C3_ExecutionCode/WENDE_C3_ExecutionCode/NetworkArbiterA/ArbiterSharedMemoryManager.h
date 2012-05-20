///////////////////////////////////////////////////////////////////////////////////
//		 File: ArbiterSharedMemoryManager.h
// Programmer: Benjamin Kurt Heiner
//       Date: May 19 2012
//
//    Purpose: This class is the single holder of shared memory objects for the 
//             arbiter code. The class is a singleton so that only one instance 
//             of them can exist at a time.
///////////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////
// Include directives
///////////////////////////////////////////////////
#include "ShmStructs.h"
#include "CSharedStruct.h"
#include "cameraMsgs.pb.h"
#include "laserMsgs.pb.h"
#include <string>
///////////////////////////////////////////////////
// google protobuf namespace
///////////////////////////////////////////////////
using namespace cameraMsgs;
using namespace laserMsgs;
using std::string;
///////////////////////////////////////////////////
// Class definition --- CArbiterSharedMemoryManager
///////////////////////////////////////////////////
class CArbiterSharedMemoryManager
{
public:
	//Singleton Manager for the shatred memory
	static CArbiterSharedMemoryManager& Instance()
	{
		// create singleton
		static CArbiterSharedMemoryManager singleton;
		// return reference to singleton
		return singleton;
	}
	// destructor
	~CArbiterSharedMemoryManager(void);							

/////////////////////////////////////////////////////////////
// Shared memory containters
/////////////////////////////////////////////////////////////
public:
	CSharedStruct<LASER_POINT_DIRECTION_SHM> m_LaserCommand;
	CSharedStruct<LASER_STATUS_MSG_SHM>		 C3LaserStatus;
	CSharedStruct<CAMERA_STATUS_MSG_SHM>	 C3CameraStatus;
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 C3CameraTracks;
	CSharedStruct<CAMERA_IMAGE_MSG_SHM>		 C3CameraImage;
/////////////////////////////////////////////////////////////
// Shared memory functions
/////////////////////////////////////////////////////////////
public:
	// decodes the network camera status message
	void DecodeCameraStatusMessage(LPCTSTR strText, char* temp,DWORD size);
	// decodes the network camera track message
	void DecodeCameraTrackMessage (LPCTSTR strText, char* temp,DWORD size);
	// decodes the network camera image message
	string DecodeCameraImageMessage (LPCTSTR strText, char* temp,DWORD size);
	// decodes the network laser status message
	void DecodeLaserStatusMessage(LPCTSTR strText, char* temp,DWORD size);
private:
	// prepares the debug string
	int DecodeCameraStatusMessage_DEBUG(cameraStatus *ss, char* temp);
	// prepares the debug string
	int DecodeCameraTrackMessage_DEBUG(cameraTracks *tr, char* temp);
	// prepares the debug string
	int DecodeCameraImageMessage_DEBUG(cameraImage *im, char* temp);
	// recreates the image so that it can be consumed by GUI
	string RecreateImage(cameraImage *im);
	// prepares the debug string
	int DecodeLaserStatusMessage_DEBUG(laserStatus *ss, char* temp);
/////////////////////////////////////////////////////////////
// Private Functions... singleton items
/////////////////////////////////////////////////////////////
private:
	// Private constructor
	CArbiterSharedMemoryManager();								
	// Prevent copy-construction
	CArbiterSharedMemoryManager(const CArbiterSharedMemoryManager&);           
	// Prevent assignment
	CArbiterSharedMemoryManager& operator=(const CArbiterSharedMemoryManager&);
};