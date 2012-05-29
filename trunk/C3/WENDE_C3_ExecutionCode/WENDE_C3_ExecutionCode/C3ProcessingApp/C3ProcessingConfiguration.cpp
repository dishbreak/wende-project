/////////////////////////////////////////////////////////////////////////////////
// File name: C3ProcessingConfiguration.cpp
// Creater: Benjamin Heiner
// Date: 5/17/2012
//
// purpose: this file contains all functions for writing to and reading from 
//          the c3 processing configuration files
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Include directives
/////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "C3ProcessingConfiguration.h"
#include "tinyxml.h"
#include <iostream>
#include <fstream>
using namespace std;
/////////////////////////////////////////////////////////////////////////////////
// Function Name: C3ProcessingConfiguration 
// Purpose: Defualt destructor (clean up mem)
/////////////////////////////////////////////////////////////////////////////////
C3ProcessingConfiguration::C3ProcessingConfiguration(void)
{
	// Configuration file
	m_CfgFile = "C3Processing.xml";

	// determine if the file exists
    ifstream infile;
	infile.open (m_CfgFile.c_str(), ifstream::in);
	if (infile.good())
	{
		infile.close();
		// read away
		ReadXml();
	}
	else
	{
		// Kalman Filter Parmaeters
		PROCESS_NOISE					= 9;				// The noise in the processing
		
		// WENDE playing field setup
		WENDE_PLAYING_FIELD_RADIUS		= 0.7;				// The radius of the playing field
		WENDE_FAILURE_LINE_RADIUS		= 2.0;				// The Failure line for the WENDE POCs

		// Camera track shm strings
		SHM_C3_CAMERA_TRACK				= "SHM_C3_CAMERA_TRACK";
		SHM_C3_CAMERA_TRACK_MUTEX		= "SHM_C3_CAMERA_TRACK_MUTEX";
		SHM_C3_CAMERA_TRACK_EVENT1		= "SHM_C3_CAMERA_TRACK_EVENT1";
		SHM_C3_CAMERA_TRACK_EVENT2		= "SHM_C3_CAMERA_TRACK_EVENT2";

		// laser track shm strings
		SHM_C3_LASER_POINTING			= "SHM_C3_LASER_POINTING";
		SHM_C3_LASER_POINTING_MUTEX		= "SHM_C3_LASER_POINTING_MUTEX";
		SHM_C3_LASER_POINTING_EVENT1	= "SHM_C3_LASER_POINTING_EVENT1";	
		SHM_C3_LASER_POINTING_EVENT2	= "SHM_C3_LASER_POINTING_EVENT2";  
		
		// TODO Add logic to check if file already exists
		WriteXMLFile();
	}
}
/////////////////////////////////////////////////////////////////////////////////
// Function Name: ~C3ProcessingConfiguration 
// Purpose: Defualt destructor (clean up mem)
/////////////////////////////////////////////////////////////////////////////////
C3ProcessingConfiguration::~C3ProcessingConfiguration(void)
{
}
void C3ProcessingConfiguration::ReadXml()
{
	// Read in the file
	TiXmlDocument doc(m_CfgFile.c_str());
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
			string name = pElem->Value();
			// save this for later
			hRoot=TiXmlHandle(pElem);
			/////////////////////
			// block: connection
			/////////////////////
			pElem=hRoot.FirstChild("SHM_FILE_CAMERA_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_CAMERA_TRACK        = pElem->Attribute("FILE");
				SHM_C3_CAMERA_TRACK_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_CAMERA_TRACK_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_CAMERA_TRACK_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else { /* ERROR ???? */ }
			pElem=hRoot.FirstChild("SHM_FILE_LASER_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				SHM_C3_LASER_POINTING        = pElem->Attribute("FILE");
				SHM_C3_LASER_POINTING_MUTEX  = pElem->Attribute("MUTEX");
				SHM_C3_LASER_POINTING_EVENT1 = pElem->Attribute("EVENT_SERVER");
				SHM_C3_LASER_POINTING_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else { /* ERROR ???? */ }
			pElem=hRoot.FirstChild("KALMAN_FILTER").Element();
			if (pElem)
			{
				// Read the 2-State Kalman parameters
				PROCESS_NOISE                = atoi(pElem->Attribute("PROCESS_NOISE"));
			}
			pElem=hRoot.FirstChild("ENVIROMENT").Element();
			if (pElem)
			{
				// Read the 2-State Kalman parameters
				pElem->QueryDoubleAttribute("PLAYING_FIELD_RADIUS",&WENDE_PLAYING_FIELD_RADIUS);
				pElem->QueryDoubleAttribute("FAILURE_LINE_RADIUS",&WENDE_FAILURE_LINE_RADIUS);
			}
			else { /* ERROR ???? */ }
		}else { /* ERROR ???? */ }
	}else { /* ERROR ???? */ }
}
/////////////////////////////////////////////////////////////////////////////////
// Function Name: WriteXMLFile 
// Purpose: This funtion write the initial configuration file for settings
/////////////////////////////////////////////////////////////////////////////////
void C3ProcessingConfiguration::WriteXMLFile()
{
	char temp[256];
	// Clean up possible memory leaks... --- FIX ---
	TiXmlDocument doc;  
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc.LinkEndChild( decl );  

	TiXmlElement * root = new TiXmlElement( "C3Processing" );  
	doc.LinkEndChild( root );  

	TiXmlComment * comment1 = new TiXmlComment();
	comment1->SetValue("Settings for the WENDE C3 Processing Shared Memory Strings");
	root->LinkEndChild( comment1 );  

	TiXmlElement * shmFileCamera = new TiXmlElement( "SHM_FILE_CAMERA_TRACK" );  
	root->LinkEndChild( shmFileCamera );  
	shmFileCamera->SetAttribute("FILE",         SHM_C3_CAMERA_TRACK.c_str());
	shmFileCamera->SetAttribute("MUTEX",		SHM_C3_CAMERA_TRACK_MUTEX.c_str());
	shmFileCamera->SetAttribute("EVENT_SERVER", SHM_C3_CAMERA_TRACK_EVENT1.c_str());
	shmFileCamera->SetAttribute("EVENT_CLIENT", SHM_C3_CAMERA_TRACK_EVENT2.c_str());

	TiXmlElement * shmFileLaser = new TiXmlElement( "SHM_FILE_LASER_TRACK" );  
	root->LinkEndChild( shmFileLaser );  
	shmFileLaser->SetAttribute("FILE",         SHM_C3_LASER_POINTING.c_str());
	shmFileLaser->SetAttribute("MUTEX",        SHM_C3_LASER_POINTING_MUTEX.c_str());
	shmFileLaser->SetAttribute("EVENT_SERVER", SHM_C3_LASER_POINTING_EVENT1.c_str());
	shmFileLaser->SetAttribute("EVENT_CLIENT", SHM_C3_LASER_POINTING_EVENT2.c_str());

	TiXmlComment * comment2 = new TiXmlComment();
	comment2->SetValue("Settings for the WENDE C3 2-State Kalman Filter");
	root->LinkEndChild( comment2 );    

	TiXmlElement * kalmanFilter = new TiXmlElement( "KALMAN_FILTER" );  
	root->LinkEndChild( kalmanFilter );  
	_itoa(PROCESS_NOISE,temp,10);
	kalmanFilter->SetAttribute("PROCESS_NOISE", temp);
	
	TiXmlComment * comment3= new TiXmlComment();
	comment3->SetValue("Settings for the WENDE ENVIROMENT SETTINGS");
	root->LinkEndChild( comment3 );

	TiXmlElement *  playingFieldRadius= new TiXmlElement( "ENVIROMENT" );  
	root->LinkEndChild( playingFieldRadius );  
	playingFieldRadius->SetDoubleAttribute("PLAYING_FIELD_RADIUS", WENDE_PLAYING_FIELD_RADIUS);
	playingFieldRadius->SetDoubleAttribute("FAILURE_LINE_RADIUS", WENDE_FAILURE_LINE_RADIUS);
	
	doc.SaveFile( m_CfgFile.c_str() ); 
}
