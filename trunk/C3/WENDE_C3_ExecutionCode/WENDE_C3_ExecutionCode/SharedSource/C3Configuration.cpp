#include "StdAfx.h"
#include "C3Configuration.h"
#include "tinyxml.h"
#include <iostream>
#include <fstream>
using namespace std;

C3Configuration::C3Configuration(void)
{
	// Configuration file
	CfgFile = "C3ConfigurationFile.xml";

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
		// Kalman Filter Parmaeters
		PROCESS_NOISE					= 9;				// The noise in the processing
		// WENDE playing field setup
		WENDE_PLAYING_FIELD_RADIUS		= 0.7;				// The radius of the playing field
		WENDE_FAILURE_LINE_RADIUS		= 2.0;				// The Failure line for the WENDE POCs
		//Laser status messages
		SHM_C3_LASER_STATUS				= "SHM_C3_LASER_STATUS";
		SHM_C3_LASER_STATUS_MUTEX		= "SHM_C3_LASER_STATUS_MUTEX";
		SHM_C3_LASER_STATUS_EVENT1		= "SHM_C3_LASER_STATUS_EVENT1";
		SHM_C3_LASER_STATUS_EVENT2		= "SHM_C3_LASER_STATUS_EVENT2";
		//Process interface SHM
		SHM_C3_PROCESSING_STATUS         = "SHM_C3_PROCESSING_STATUS";			// Processing IMAGE SHM File Name
		SHM_C3_PROCESSING_STATUS_MUTEX   = "SHM_C3_PROCESSING_STATUS_MUTEX"; 			// Processing IMAGE MUTEX File Name
		SHM_C3_PROCESSING_STATUS_EVENT1  = "SHM_C3_PROCESSING_STATUS_EVENT1";			// Processing IMAGE Event1 File Name
		SHM_C3_PROCESSING_STATUS_EVENT2  = "SHM_C3_PROCESSING_STATUS_EVENT2";			// Processing IMAGE Event2 File Name
		//Process debug interface SHM
		SHM_C3_PROCESSING_STATUS         = "SHM_C3_PROCESSING_DEBUG_STATUS";			// Processing IMAGE SHM File Name
		SHM_C3_PROCESSING_STATUS_MUTEX   = "SHM_C3_PROCESSING_DEBUG_MUTEX"; 			// Processing IMAGE MUTEX File Name
		SHM_C3_PROCESSING_STATUS_EVENT1  = "SHM_C3_PROCESSING_DEBUG_EVENT1";			// Processing IMAGE Event1 File Name
		SHM_C3_PROCESSING_STATUS_EVENT2  = "SHM_C3_PROCESSING_DEBUG_EVENT2";			// Processing IMAGE Event2 File Name
		// laser track shm strings
		SHM_C3_LASER_POINTING			= "SHM_C3_LASER_POINTING";
		SHM_C3_LASER_POINTING_MUTEX		= "SHM_C3_LASER_POINTING_MUTEX";
		SHM_C3_LASER_POINTING_EVENT1	= "SHM_C3_LASER_POINTING_EVENT1";	
		SHM_C3_LASER_POINTING_EVENT2	= "SHM_C3_LASER_POINTING_EVENT2";  
		//Camera status messages
		SHM_C3_CAMERA_STATUS			= "SHM_C3_CAMERA_STATUS";
		SHM_C3_CAMERA_STATUS_MUTEX		= "SHM_C3_CAMERA_STATUS_MUTEX";
		SHM_C3_CAMERA_STATUS_EVENT1		= "SHM_C3_CAMERA_STATUS_EVENT1";
		SHM_C3_CAMERA_STATUS_EVENT2		= "SHM_C3_CAMERA_STATUS_EVENT2";
		//Camera track
		SHM_C3_CAMERA_TRACK				= "SHM_C3_CAMERA_TRACK";
		SHM_C3_CAMERA_TRACK_MUTEX		= "SHM_C3_CAMERA_TRACK_MUTEX";
		SHM_C3_CAMERA_TRACK_EVENT1		= "SHM_C3_CAMERA_TRACK_EVENT1";
		SHM_C3_CAMERA_TRACK_EVENT2		= "SHM_C3_CAMERA_TRACK_EVENT2";
		//Camera image
		SHM_C3_CAMERA_IMAGE				= "SHM_C3_CAMERA_IMAGE";
		SHM_C3_CAMERA_IMAGE_MUTEX		= "SHM_C3_CAMERA_IMAGE_MUTEX";
		SHM_C3_CAMERA_IMAGE_EVENT1		= "SHM_C3_CAMERA_IMAGE_EVENT1";
		SHM_C3_CAMERA_IMAGE_EVENT2		= "SHM_C3_CAMERA_IMAGE_EVENT2";

		// network
		ConnectionArbiter.ip = "192.168.1.70";
		ConnectionArbiter.port= 4444;
		//driver
		ConnectionDriver.ip = "192.168.1.70";
		ConnectionDriver.port = 4447;
		// DEBUG ITEMS
		isShowDebugPannel           = false;
		driverStartPath			= "ServerSocket.exe";
		processingStartPath		= "C3ProcessingApp.exe";
		arbiterStartPath		= "NetworkArbiter.exe";
		// laser height
		LASER_HEIGHT				= 1.5;//meters
		WriteXMLFile();
	}
}

C3Configuration::~C3Configuration(void)
{
}
void C3Configuration::ReadXMLFile()
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
			//CString name=pElem->Value();
			// save this for later
			hRoot=TiXmlHandle(pElem);
			/////////////////////
			// block: connection
			/////////////////////
			pElem=hRoot.FirstChild("ConnectionArbiter").Element();
			if (pElem)
			{
				ConnectionArbiter.ip   = pElem->Attribute("ip");
				ConnectionArbiter.port = atoi(pElem->Attribute("port"));	
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("ConnectionDriver").Element();
			if (pElem)
			{
				ConnectionDriver.ip   = pElem->Attribute("ip");
				ConnectionDriver.port = atoi(pElem->Attribute("port"));	
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_STATUS        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("StartupPaths").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				arbiterStartPath    = pElem->Attribute("Arbiter");
				processingStartPath = pElem->Attribute("Processing");
				driverStartPath     = pElem->Attribute("Driver");
			}
			else{ /* ERROR ???? */}	
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_TRACK        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_TRACK_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_TRACK_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_TRACK_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_IMAGE").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_IMAGE        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_IMAGE_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_IMAGE_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_IMAGE_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_STATUS        = pElem->Attribute("FILE");
				SHM_C3_LASER_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_POINTING        = pElem->Attribute("FILE");
				SHM_C3_LASER_POINTING_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_POINTING_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_POINTING_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("SHM_FILE_PROCESSING_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_PROCESSING_STATUS        = pElem->Attribute("FILE");
				SHM_C3_PROCESSING_STATUS_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_PROCESSING_STATUS_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_PROCESSING_STATUS_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("SHM_FILE_PROCESSING_DEBUG_STATUS").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_PROCESSING_DEBUG_STATUS	= pElem->Attribute("FILE");
				SHM_C3_PROCESSING_DEBUG_MUTEX	= pElem->Attribute("MUTEX");
				SHM_C3_PROCESSING_DEBUG_EVENT1	= pElem->Attribute("EVENT_SERVER");
				SHM_C3_PROCESSING_DEBUG_EVENT2	= pElem->Attribute("EVENT_CLIENT");
			}
			else{ /* ERROR ???? */}
			pElem=hRoot.FirstChild("KALMAN_FILTER").Element();
			if (pElem)
			{
				// Read the 2-State Kalman parameters
				PROCESS_NOISE                = atoi(pElem->Attribute("PROCESS_NOISE"));
			}
			else { /* ERROR ???? */ }
			pElem=hRoot.FirstChild("LASER_PARAMS").Element();
			if (pElem)
			{
				// Read the 2-State Kalman parameters
				LASER_HEIGHT                = atoi(pElem->Attribute("height"));
			}
			else { /* ERROR ???? */ }
			pElem=hRoot.FirstChild("ENVIROMENT").Element();
			if (pElem)
			{
				// Read the 2-State Kalman parameters
				pElem->QueryDoubleAttribute("PLAYING_FIELD_RADIUS",&WENDE_PLAYING_FIELD_RADIUS);
				pElem->QueryDoubleAttribute("FAILURE_LINE_RADIUS",&WENDE_FAILURE_LINE_RADIUS);
			}
			else { /* ERROR ???? */ }
			pElem=hRoot.FirstChild("DEBUG_PANNEL").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				pElem->QueryBoolAttribute("ENABLE",&isShowDebugPannel);
			}
			else{/* ERROR ????*/}
		}
		else{ /* ERROR ???? */}
	}
	else{ /* ERROR ???? */}
}
void C3Configuration::WriteXMLFile()
{
	char temp[256];
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

	TiXmlElement * cxn = new TiXmlElement( "ConnectionArbiter" );  
	root->LinkEndChild( cxn );  
	cxn->SetAttribute("ip", ConnectionArbiter.ip.c_str());
	cxn->SetAttribute("port", ConnectionArbiter.port); // floating point attrib

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

	TiXmlElement * shmFileProcessingStatus = new TiXmlElement( "SHM_FILE_PROCESSING_STATUS" );  
	root->LinkEndChild( shmFileProcessingStatus );  
	shmFileProcessingStatus->SetAttribute("FILE",         SHM_C3_PROCESSING_STATUS.c_str());
	shmFileProcessingStatus->SetAttribute("MUTEX",        SHM_C3_PROCESSING_STATUS_MUTEX.c_str());
	shmFileProcessingStatus->SetAttribute("EVENT_SERVER", SHM_C3_PROCESSING_STATUS_EVENT1.c_str());
	shmFileProcessingStatus->SetAttribute("EVENT_CLIENT", SHM_C3_PROCESSING_STATUS_EVENT2.c_str());

	TiXmlElement * shmFileLaserTrack = new TiXmlElement( "SHM_FILE_LASER_TRACK" );  
	root->LinkEndChild( shmFileLaserTrack );  
	shmFileLaserTrack->SetAttribute("FILE",         SHM_C3_LASER_POINTING.c_str());
	shmFileLaserTrack->SetAttribute("MUTEX",        SHM_C3_LASER_POINTING_MUTEX.c_str());
	shmFileLaserTrack->SetAttribute("EVENT_SERVER", SHM_C3_LASER_POINTING_EVENT1.c_str());
	shmFileLaserTrack->SetAttribute("EVENT_CLIENT", SHM_C3_LASER_POINTING_EVENT2.c_str());

	TiXmlElement * shmProcessingDebug= new TiXmlElement( "SHM_C3_PROCESSING_DEBUG_STATUS" );  
	root->LinkEndChild( shmProcessingDebug );  
	shmProcessingDebug->SetAttribute("FILE",         SHM_C3_PROCESSING_DEBUG_STATUS.c_str());
	shmProcessingDebug->SetAttribute("MUTEX",        SHM_C3_PROCESSING_DEBUG_MUTEX.c_str());
	shmProcessingDebug->SetAttribute("EVENT_SERVER", SHM_C3_PROCESSING_DEBUG_EVENT1.c_str());
	shmProcessingDebug->SetAttribute("EVENT_CLIENT", SHM_C3_PROCESSING_DEBUG_EVENT2.c_str());

	TiXmlComment * comment5 = new TiXmlComment();
	comment5->SetValue("Debug Configuration items" );  
	root->LinkEndChild( comment5 );  
	TiXmlComment * comment6 = new TiXmlComment();
	comment6->SetValue("Settings for the WENDE C3 2-State Kalman Filter");
	root->LinkEndChild( comment6 );    

	TiXmlElement * kalmanFilter = new TiXmlElement( "KALMAN_FILTER" );  
	root->LinkEndChild( kalmanFilter );  
	_itoa(PROCESS_NOISE,temp,10);
	kalmanFilter->SetAttribute("PROCESS_NOISE", temp);
	
	TiXmlComment * comment7= new TiXmlComment();
	comment7->SetValue("Settings for the WENDE ENVIROMENT SETTINGS");
	root->LinkEndChild( comment7 );

	TiXmlElement *  playingFieldRadius= new TiXmlElement( "ENVIROMENT" );  
	root->LinkEndChild( playingFieldRadius );  
	playingFieldRadius->SetDoubleAttribute("PLAYING_FIELD_RADIUS", WENDE_PLAYING_FIELD_RADIUS);
	playingFieldRadius->SetDoubleAttribute("FAILURE_LINE_RADIUS", WENDE_FAILURE_LINE_RADIUS);

	TiXmlComment * comment8 = new TiXmlComment();
	comment8->SetValue("Debug Configuration items" );  
	root->LinkEndChild( comment8 );  

	TiXmlElement * debugPannel = new TiXmlElement( "DEBUG_PANNEL" );  
	root->LinkEndChild( debugPannel );  
	debugPannel->SetAttribute("ENABLE", isShowDebugPannel);

	TiXmlComment * comment9 = new TiXmlComment();
	comment9->SetValue("Settings for the WENDE C3 Driver (DEBUG)     " );  
	root->LinkEndChild( comment9 );  
	TiXmlComment * comment10 = new TiXmlComment();
	comment10->SetValue("IP Address of the server              " );  
	root->LinkEndChild( comment10 );
	TiXmlComment * comment11 = new TiXmlComment();
	comment11->SetValue("Port is the starting port for messages" );  
	root->LinkEndChild( comment11 );  

	TiXmlElement * cxnd = new TiXmlElement( "ConnectionDriver" );  
	root->LinkEndChild( cxnd );  
	cxnd->SetAttribute("ip", ConnectionDriver.ip.c_str());
	cxnd->SetAttribute("port", ConnectionDriver.port); // floating point attrib

	TiXmlElement * stPaths = new TiXmlElement( "StartupPaths" );  
	root->LinkEndChild( stPaths );  
	stPaths->SetAttribute("Arbiter", arbiterStartPath.c_str());
	stPaths->SetAttribute("Processing", processingStartPath.c_str());
	stPaths->SetAttribute("Driver", driverStartPath.c_str());

	TiXmlElement * lParams = new TiXmlElement( "LASER_PARAMS" );  
	root->LinkEndChild( lParams );  
	lParams->SetAttribute("height", LASER_HEIGHT); // floating point attrib

	doc.SaveFile( CfgFile.c_str() ); 
}
