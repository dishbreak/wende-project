#pragma once
#define SHM_STRUCTS_H
#define SHM_MAX_STATUS_TEXT 256
#define SHM_MAX_TRACKS      10
#define SHM_MAX_LASERS      10
/// 1024w * 1024h * 3p
#define SHM_MAX_IMAGE_SIZE  3145728

typedef struct{
	DWORD  ProcessID;
	DWORD  Time;
	
	UINT32 PacketNumber;
	UINT32 LaserOnOff;
	UINT32 PWM_AZ;
	UINT32 PWM_EL;
	// TODO PERFORMANCE COUNTER - FIX
}LASER_POINT_DIRECTION_SHM;

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


typedef struct{
	INT32 X;
	INT32 Y;
}CAMERA_TRACK;

typedef struct{
	DWORD		 ProcessID;
	DWORD        Time;
	
	UINT32	     PacketNumber;
	UINT32		 Status;
	UINT32		 LaserOnOf;
	UINT32	     ValidTracks;
	UINT32       ValidLasers;
	UINT32       SubsystemId;

	CAMERA_TRACK Tracks[SHM_MAX_TRACKS];
	CAMERA_TRACK Lasers[SHM_MAX_LASERS];
	// TODO PERFORMANCE COUNTER - FIX
}CAMERA_TRACK_MSG_SHM;

typedef struct{
	DWORD		 ProcessID;
	DWORD        Time;

	UINT32		 PacketNumber;
	UINT32		 Channels;
	
	//SIZE		 CameraSize;

	//BYTE		 ImageData[SHM_MAX_IMAGE_SIZE];
	// TODO PERFORMANCE COUNTER - FIX
}CAMERA_IMAGE_MSG_SHM;