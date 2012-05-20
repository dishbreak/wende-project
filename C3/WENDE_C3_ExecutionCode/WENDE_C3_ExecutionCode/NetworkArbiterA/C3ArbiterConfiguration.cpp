#include "StdAfx.h"
#include "C3ArbiterConfiguration.h"
#include "tinyxml.h"
#include <iostream>
#include <fstream>
using namespace std;

C3ArbiterConfiguration::C3ArbiterConfiguration(void)
{
	// Configuration file
	CfgFile = "C3Arbiter.xml";

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
		
		// TODO Add logic to check if file already exists
		WriteXMLFile();
	}
}

C3ArbiterConfiguration::~C3ArbiterConfiguration(void)
{
}
void C3ArbiterConfiguration::ReadXMLFile()
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
			CString name=pElem->Value();
			// save this for later
			hRoot=TiXmlHandle(pElem);
			/////////////////////
			// block: connection
			/////////////////////
			pElem=hRoot.FirstChild("Connection").Element();
			if (pElem)
			{
				Connection.ip   = pElem->Attribute("ip");
				Connection.port = atoi(pElem->Attribute("port"));	
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_STATUS        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_TRACK        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_TRACK_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_TRACK_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_TRACK_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_IMAGE").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_IMAGE        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_IMAGE_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_IMAGE_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_IMAGE_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_STATUS        = pElem->Attribute("FILE");
				SHM_C3_LASER_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_POINTING        = pElem->Attribute("FILE");
				SHM_C3_LASER_POINTING_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_POINTING_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_POINTING_EVENT2 = pElem->Attribute("EVENT_CLIENT");
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
	else
	{
		// ERROR ????	
	}
}
void C3ArbiterConfiguration::WriteXMLFile()
{
	// Clean up possible memory leaks... --- FIX ---
	TiXmlDocument doc;  
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  

	TiXmlElement * root = new TiXmlElement( "NetworkArbiter" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment1 = new TiXmlComment();
	comment1->SetValue("Settings for the WENDE C3 Arbiter     " );  
	root->LinkEndChild( comment1 );  
	TiXmlComment * comment2 = new TiXmlComment();
	comment2->SetValue("IP Address of the server              " );  
	root->LinkEndChild( comment2 );
	TiXmlComment * comment3 = new TiXmlComment();
	comment3->SetValue("Port is the starting port for messages" );  
	root->LinkEndChild( comment3 );  

	TiXmlElement * cxn = new TiXmlElement( "Connection" );  
	root->LinkEndChild( cxn );  
	cxn->SetAttribute("ip", Connection.ip.c_str());
	cxn->SetAttribute("port", Connection.port); // floating point attrib

	TiXmlComment * comment4 = new TiXmlComment();
	comment4->SetValue("Settings for the WENDE C3 Processing Shared Memory Strings");
	root->LinkEndChild( comment4 );  

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

	TiXmlComment * comment5 = new TiXmlComment();
	comment5->SetValue("Debug Configuration items" );  
	root->LinkEndChild( comment5 );  

	doc.SaveFile( CfgFile.c_str() ); 
}
