// C3GUIStub.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CSharedStruct.h"
#include "ShmStructs.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CSharedStruct<CAMERA_STATUS_MSG_SHM>	 m_CameraStatus;
	CSharedStruct<CAMERA_TRACK_MSG_SHM>		 m_CameraTracks;
	CSharedStruct<CAMERA_IMAGE_MSG_SHM>		 m_CameraImage;
	
	return 0;
}

