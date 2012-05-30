/////////////////////////////////////////////////////////////////////////////////
// File name: ShmStructs
// Creater: Benjamin Heiner
// Date: 5/17/2012
//
// purpose: this file contains all the internal message structures used in processing
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <stdio.h>  /* defines FILENAME_MAX */
/////////////////////////////////////////////////////////////////////////////////
// TYPE DEFINES
/////////////////////////////////////////////////////////////////////////////////
typedef unsigned long       DWORD;
typedef DWORD				COLORREF;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef double              DOUBLE;
typedef signed char         INT8;
typedef signed short        INT16;
typedef signed int          INT32;
typedef signed __int64      INT64;
typedef unsigned char       UINT8;
typedef unsigned short      UINT16;
typedef unsigned int        UINT32;
typedef unsigned __int64    UINT64;
typedef char				CHAR;
typedef short				SHORT;
typedef long				LONG;
/////////////////////////////////////////////////////////////////////////////////
// Struct: max sizes
/////////////////////////////////////////////////////////////////////////////////
#define SHM_MAX_STATUS_TEXT		256			// Max stutus text size from the camera
#define SHM_MAX_TRACKS			10			// Max number of internal tracks
#define SHM_MAX_LASERS			10			// Max number of laser tracks
#define SHM_MAX_IMAGE_NAME		FILENAME_MAX// Max characters in the iamge name
#define SHM_MAX_IMAGE_WIDTH		320  		// Max image size in width direction
#define SHM_MAX_IMAGE_HEIGHT	240	    	// Max image size in the height direction
#define SHM_MAX_IAMGE_CHANNELS	3			// Max image channels 
// Max image size
#define SHM_MAX_IMAGE_SIZE		SHM_MAX_IMAGE_WIDTH*SHM_MAX_IMAGE_HEIGHT*SHM_MAX_IAMGE_CHANNELS	

/////////////////////////////////////////////////////////////////////////////////
// Struct: C3Size
//
// purpose: Size of an imaage (x,y ---> w.h)
/////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	DWORD Clients;
} SHM_INFO_STRUCT;
/////////////////////////////////////////////////////////////////////////////////
// Struct: C3Size
//
// purpose: Size of an imaage (x,y ---> w.h)
/////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    DWORD        cx;
    DWORD        cy;
} C3SIZE;
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

	SHM_INFO_STRUCT		ShmInfo;			// Shared SHM information
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
} C3_TRACK_POINT;
/////////////////////////////////////////////////////////////////////////////////
// Struct: C3_TRACK_POINT_DOUBLE
//
// purpose: defines the structure for representing C3 Tracks
/////////////////////////////////////////////////////////////////////////////////
typedef union
{
	struct
	{
		DOUBLE		 X;			// X location (offset) 
		DOUBLE		 Y;			// Y location (offset)
	};
	struct
	{
		DOUBLE		 AZ;		// AZ Location
		DOUBLE		 EL;		// EL Location
	};
} C3_TRACK_POINT_DOUBLE;
/////////////////////////////////////////////////////////////////////////////////
// Struct: LASER_POINTING_MSG_SHM
//
// purpose: defines the structure for internal message passing of camera track
//          commands
/////////////////////////////////////////////////////////////////////////////////
typedef struct{
	DWORD		 ProcessID;						// process id of generator
	DWORD        Time;							// time of the message (based on camera)
	
	UINT32	     PacketNumber;					// packet number
	UINT32		 Status;
	UINT32		 LaserOnOf;
	UINT32	     ValidTracks;
	UINT32       ValidLasers;
	UINT32       SubsystemId;

	C3_TRACK_POINT Tracks[SHM_MAX_TRACKS];
	C3_TRACK_POINT Lasers[SHM_MAX_LASERS];
	// TODO PERFORMANCE COUNTER - FIX
	SHM_INFO_STRUCT		ShmInfo;			// Shared SHM information
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
	CHAR		 imagePath[SHM_MAX_IMAGE_NAME];		// the image path			
	// TODO PERFORMANCE COUNTER - FIX
	SHM_INFO_STRUCT		ShmInfo;					// Shared SHM information
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

	C3_TRACK_POINT	 PointLocation;					// relative pointing command
	// TODO PERFORMANCE COUNTER - FIX
	SHM_INFO_STRUCT		ShmInfo;					// Shared SHM information
}LASER_POINT_DIRECTION_SHM;
/////////////////////////////////////////////////////////////////////////////////
// Struct: UPixel
//
// purpose: allows access to rgb data in a image
/////////////////////////////////////////////////////////////////////////////////
typedef union
{
	COLORREF c;
	struct
	{
#ifdef LITTLEENDIAN
		unsigned char cBlue;
		unsigned char cGreen;
		unsigned char cRed;
		unsigned char crsvd;
#else
		unsigned char crsvd;
		unsigned char cRed;
		unsigned char cGreen;
		unsigned char cBlue;
#endif
	} chars;
} UPixel;
/////////////////////////////////////////////////////////////////////////////////
// Struct: LASER_STATUS_MSG_SHM
//
// purpose: defines the structure for internal message passing of laser status
//          commands
/////////////////////////////////////////////////////////////////////////////////
typedef struct {
	DWORD  ProcessID;
	DWORD  Time;

	UINT32 PacketNumber;
	UINT32 Status;
	
	UINT32 ValidChars;
	UINT32 SubsystemId;
	
	CHAR   textStr[SHM_MAX_STATUS_TEXT];
	// TODO PERFORMANCE COUNTER - FIX

	SHM_INFO_STRUCT		ShmInfo;			// Shared SHM information
}LASER_STATUS_MSG_SHM;