/////////////////////////////////////////////////////////////////////////////////
// File name: ShmStructs
// Creater: Benjamin Heiner
// Date: 5/17/2012
//
// purpose: this file contains all the internal message structures used in processing
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#define SHM_MAX_STATUS_TEXT		256			// Max stutus text size from the camera
#define SHM_MAX_TRACKS			10			// Max number of internal tracks
#define SHM_MAX_LASERS			10			// Max number of laser tracks
#define SHM_MAX_IMAGE_WIDTH		320			// Max image size in width direction
#define SHM_MAX_IMAGE_HEIGHT	240			// Max image size in the height direction
#define SHM_MAX_IAMGE_CHANNELS	3			// Max image channels 
// Max image size
#define SHM_MAX_IMAGE_SIZE		SHM_MAX_IMAGE_WIDTH*SHM_MAX_IMAGE_HEIGHT*SHM_MAX_IAMGE_CHANNELS	

/////////////////////////////////////////////////////////////////////////////////
// Struct: CAMERA_STATUS_MSG_SHM
//
// purpose: defines the structure for internal message passing of camera status
//          commands
/////////////////////////////////////////////////////////////////////////////////
typedef struct {
	DWORD  ProcessID;
	DWORD  Time;

	UINT32 PacketNumber;
	UINT32 Status;
	UINT32 LaserOnOf;
	
	UINT32 ValidChars;
	UINT32 SubsystemId;
	
	CHAR   textStr[SHM_MAX_STATUS_TEXT];
	// TODO PERFORMANCE COUNTER - FIX
}CAMERA_STATUS_MSG_SHM;
/////////////////////////////////////////////////////////////////////////////////
// Struct: C3_TRACK
//
// purpose: defines the structure for representing C3 Tracks
/////////////////////////////////////////////////////////////////////////////////
typedef union
{
	struct
	{
		INT32		 X;			// X location (offset) 
		INT32		 Y;			// Y location (offset)
	};
	struct
	{
		INT32		 AZ;		// AZ Location
		INT32		 EL;		// EL Location
	};
} C3_TRACK;
/////////////////////////////////////////////////////////////////////////////////
// Struct: LASER_POINTING_MSG_SHM
//
// purpose: defines the structure for internal message passing of camera track
//          commands
/////////////////////////////////////////////////////////////////////////////////
typedef struct{
	DWORD		 ProcessID;
	DWORD        Time;
	
	UINT32	     PacketNumber;
	UINT32		 Status;
	UINT32		 LaserOnOf;
	UINT32	     ValidTracks;
	UINT32       ValidLasers;
	UINT32       SubsystemId;

	C3_TRACK Tracks[SHM_MAX_TRACKS];
	C3_TRACK Lasers[SHM_MAX_LASERS];
	// TODO PERFORMANCE COUNTER - FIX
}CAMERA_TRACK_MSG_SHM;
/////////////////////////////////////////////////////////////////////////////////
// Struct: CAMERA_IMAGE_MSG_SHM
//
// purpose: defines the structure for internal message passing of camer image
//          commands
/////////////////////////////////////////////////////////////////////////////////
typedef struct{
	DWORD		 ProcessID;							// process id of generator
	DWORD        Time;								// time of the message (based on camera)

	UINT32		 PacketNumber;						// packet number
	UINT32		 Channels;							// number of channels in the image (i.e. 3 RGB)
	
	SIZE		 CameraSize;						// Number of valid bytes in the image data
													// (i.e. W*H*C=size)		
				
	BYTE		 ImageData[SHM_MAX_IMAGE_SIZE];		// Image data (RGB bytes)
	// TODO PERFORMANCE COUNTER - FIX
}CAMERA_IMAGE_MSG_SHM;
/////////////////////////////////////////////////////////////////////////////////
// Struct: LASER_POINTING_MSG_SHM
//
// purpose: defines the structure for internal message passing of laser pointing
//          commands
/////////////////////////////////////////////////////////////////////////////////
typedef struct{
	DWORD		 ProcessID;							// process id of generator
	DWORD        Time;								// time of the message (based on camera)

	UINT32		 PacketNumber;						// packet number
	UINT32		 LaserOnOff;						// Bool for Laser ON or OFF (0 or 1)

	C3_TRACK	 PointLocation;						// relative pointing command
	// TODO PERFORMANCE COUNTER - FIX
}LASER_POINT_DIRECTION_SHM;
