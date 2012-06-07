#include "StdAfx.h"
#include "GUIConfiguration.h"
#include "tinyxml.h"
#include <iostream>
#include <fstream>
using namespace std;

CGUIConfiguration::CGUIConfiguration(void)
{
	// Configuration file
	CfgFile = "C3GUI.xml";

	// determine if the file exists
    ifstream infile;
	infile.open (CfgFile.c_str(), ifstream::in);
	if (infile.good())
	{
		infile.close();
		// read away
		ReadXMLFile();
	}
	else
	{
		//Laser status messages
		SHM_C3_LASER_STATUS			= "SHM_C3_LASER_STATUS";
		SHM_C3_LASER_STATUS_MUTEX	= "SHM_C3_LASER_STATUS_MUTEX";
		SHM_C3_LASER_STATUS_EVENT1	= "SHM_C3_LASER_STATUS_EVENT1";
		SHM_C3_LASER_STATUS_EVENT2	= "SHM_C3_LASER_STATUS_EVENT2";
		// laser track shm strings
		SHM_C3_LASER_POINTING		= "SHM_C3_LASER_POINTING";
		SHM_C3_LASER_POINTING_MUTEX	= "SHM_C3_LASER_POINTING_MUTEX";
		SHM_C3_LASER_POINTING_EVENT1= "SHM_C3_LASER_POINTING_EVENT1";	
		SHM_C3_LASER_POINTING_EVENT2= "SHM_C3_LASER_POINTING_EVENT2";  
		//Camera status messages
		SHM_C3_CAMERA_STATUS		= "SHM_C3_CAMERA_STATUS";
		SHM_C3_CAMERA_STATUS_MUTEX	= "SHM_C3_CAMERA_STATUS_MUTEX";
		SHM_C3_CAMERA_STATUS_EVENT1	= "SHM_C3_CAMERA_STATUS_EVENT1";
		SHM_C3_CAMERA_STATUS_EVENT2	= "SHM_C3_CAMERA_STATUS_EVENT2";
		//Camera track
		SHM_C3_CAMERA_TRACK			= "SHM_C3_CAMERA_TRACK";
		SHM_C3_CAMERA_TRACK_MUTEX	= "SHM_C3_CAMERA_TRACK_MUTEX";
		SHM_C3_CAMERA_TRACK_EVENT1	= "SHM_C3_CAMERA_TRACK_EVENT1";
		SHM_C3_CAMERA_TRACK_EVENT2	= "SHM_C3_CAMERA_TRACK_EVENT2";
		//Camera image
		SHM_C3_CAMERA_IMAGE			= "SHM_C3_CAMERA_IMAGE";
		SHM_C3_CAMERA_IMAGE_MUTEX	= "SHM_C3_CAMERA_IMAGE_MUTEX";
		SHM_C3_CAMERA_IMAGE_EVENT1	= "SHM_C3_CAMERA_IMAGE_EVENT1";
		SHM_C3_CAMERA_IMAGE_EVENT2	= "SHM_C3_CAMERA_IMAGE_EVENT2";
		// DEBUG ITEMS
		isShowDebugPannel           = false;
		// TODO Add logic to check if file already exists
		WriteXMLFile();
	}
}

CGUIConfiguration::~CGUIConfiguration(void)
{
}
void CGUIConfiguration::ReadXMLFile()
{
	// Read in the file
	TiXmlDocument doc(CfgFile.c_str());
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);
		///////////////
		// block: name
		///////////////
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (pElem)
		{
			string name=pElem->Value();
			// save this for later
			hRoot=TiXmlHandle(pElem);
			/////////////////////
			// block: connection
			/////////////////////
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_STATUS        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{/* ERROR ????*/}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_TRACK        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_TRACK_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_TRACK_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_TRACK_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{/* ERROR ????*/}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_IMAGE").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_IMAGE        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_IMAGE_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_IMAGE_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_IMAGE_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{/* ERROR ????*/}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_STATUS        = pElem->Attribute("FILE");
				SHM_C3_LASER_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{/* ERROR ????*/}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_POINTING        = pElem->Attribute("FILE");
				SHM_C3_LASER_POINTING_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_POINTING_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_POINTING_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{/* ERROR ????*/}
			pElem=hRoot.FirstChild("DEBUG_PANNEL").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				pElem->QueryBoolAttribute("ENABLE",&isShowDebugPannel);
			}
			else{/* ERROR ????*/}
		}
		else
		{
			// ERROR ????
		}
	}
	else
	{
		// ERROR ????	
	}
}
void CGUIConfiguration::WriteXMLFile()
{
	// Clean up possible memory leaks... --- FIX ---
	TiXmlDocument doc;  
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  

	TiXmlElement * root = new TiXmlElement( "C3GUI" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment1 = new TiXmlComment();
	comment1->SetValue("Settings for the WENDE C3 Processing Shared Memory Strings");
	root->LinkEndChild( comment1 );  

	TiXmlElement * shmFileCameraStatus = new TiXmlElement( "SHM_FILE_CAMERA_STATUS" );  
	root->LinkEndChild( shmFileCameraStatus );  
	shmFileCameraStatus->SetAttribute("FILE",         SHM_C3_CAMERA_STATUS.c_str());
	shmFileCameraStatus->SetAttribute("MUTEX",		  SHM_C3_CAMERA_STATUS_MUTEX.c_str());
	shmFileCameraStatus->SetAttribute("EVENT_SERVER", SHM_C3_CAMERA_STATUS_EVENT1.c_str());
	shmFileCameraStatus->SetAttribute("EVENT_CLIENT", SHM_C3_CAMERA_STATUS_EVENT2.c_str());

	TiXmlElement * shmFileCameraTrack = new TiXmlElement( "SHM_FILE_CAMERA_TRACK" );  
	root->LinkEndChild( shmFileCameraTrack );  
	shmFileCameraTrack->SetAttribute("FILE",         SHM_C3_CAMERA_TRACK.c_str());
	shmFileCameraTrack->SetAttribute("MUTEX",		 SHM_C3_CAMERA_TRACK_MUTEX.c_str());
	shmFileCameraTrack->SetAttribute("EVENT_SERVER", SHM_C3_CAMERA_TRACK_EVENT1.c_str());
	shmFileCameraTrack->SetAttribute("EVENT_CLIENT", SHM_C3_CAMERA_TRACK_EVENT2.c_str());

	TiXmlElement * shmFileCameraImage = new TiXmlElement( "SHM_FILE_CAMERA_IMAGE" );  
	root->LinkEndChild( shmFileCameraImage );  
	shmFileCameraImage->SetAttribute("FILE",         SHM_C3_CAMERA_IMAGE.c_str());
	shmFileCameraImage->SetAttribute("MUTEX",		 SHM_C3_CAMERA_IMAGE_MUTEX.c_str());
	shmFileCameraImage->SetAttribute("EVENT_SERVER", SHM_C3_CAMERA_IMAGE_EVENT1.c_str());
	shmFileCameraImage->SetAttribute("EVENT_CLIENT", SHM_C3_CAMERA_IMAGE_EVENT2.c_str());

	TiXmlElement * shmFileLaserStatus = new TiXmlElement( "SHM_FILE_LASER_STATUS" );  
	root->LinkEndChild( shmFileLaserStatus );  
	shmFileLaserStatus->SetAttribute("FILE",         SHM_C3_LASER_STATUS.c_str());
	shmFileLaserStatus->SetAttribute("MUTEX",        SHM_C3_LASER_STATUS_MUTEX.c_str());
	shmFileLaserStatus->SetAttribute("EVENT_SERVER", SHM_C3_LASER_STATUS_EVENT1.c_str());
	shmFileLaserStatus->SetAttribute("EVENT_CLIENT", SHM_C3_LASER_STATUS_EVENT2.c_str());

	TiXmlElement * shmFileLaserTrack = new TiXmlElement( "SHM_FILE_LASER_TRACK" );  
	root->LinkEndChild( shmFileLaserTrack );  
	shmFileLaserTrack->SetAttribute("FILE",         SHM_C3_LASER_POINTING.c_str());
	shmFileLaserTrack->SetAttribute("MUTEX",        SHM_C3_LASER_POINTING_MUTEX.c_str());
	shmFileLaserTrack->SetAttribute("EVENT_SERVER", SHM_C3_LASER_POINTING_EVENT1.c_str());
	shmFileLaserTrack->SetAttribute("EVENT_CLIENT", SHM_C3_LASER_POINTING_EVENT2.c_str());

	TiXmlComment * comment2 = new TiXmlComment();
	comment2->SetValue("Debug Configuration items" );  
	root->LinkEndChild( comment2 );  

	TiXmlElement * debugPannel = new TiXmlElement( "DEBUG_PANNEL" );  
	root->LinkEndChild( debugPannel );  
	debugPannel->SetAttribute("ENABLE", isShowDebugPannel);

	doc.SaveFile( CfgFile.c_str() ); 
}