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
		m_ProcessNoise					= 9; 
		// Camera track shm strings
		m_SHM_C3_CAMERA_TRACK			= "SHM_C3_CAMERA_TRACK";
		m_SHM_C3_CAMERA_TRACK_MUTEX		= "SHM_C3_CAMERA_TRACK_MUTEX";
		m_SHM_C3_CAMERA_TRACK_EVENT1	= "SHM_C3_CAMERA_TRACK_MUTEX";
		m_SHM_C3_CAMERA_TRACK_EVENT2	= "SHM_C3_CAMERA_TRACK_MUTEX";
		// laser track shm strings
		m_SHM_C3_LASER_POINTING			= "SHM_C3_LASER_POINTING";
		m_SHM_C3_LASER_POINTING_MUTEX	= "SHM_C3_LASER_POINTING_MUTEX";
		m_SHM_C3_LASER_POINTING_EVENT1	= "SHM_C3_LASER_POINTING_EVENT1";	
		m_SHM_C3_LASER_POINTING_EVENT2	= "SHM_C3_LASER_POINTING_EVENT2";  
		
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
				m_SHM_C3_CAMERA_TRACK        = pElem->Attribute("FILE");
				m_SHM_C3_CAMERA_TRACK_MUTEX  = pElem->Attribute("MUTEX");
				m_SHM_C3_CAMERA_TRACK_EVENT1 = pElem->Attribute("EVENT_SERVER");
				m_SHM_C3_CAMERA_TRACK_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("SHM_FILE_LASER_TRACK").Element();
			if (pElem)
			{
				// Read the camera pointing directions
				m_SHM_C3_LASER_POINTING        = pElem->Attribute("FILE");
				m_SHM_C3_LASER_POINTING_MUTEX  = pElem->Attribute("MUTEX");
				m_SHM_C3_LASER_POINTING_EVENT1 = pElem->Attribute("EVENT_SERVER");
				m_SHM_C3_LASER_POINTING_EVENT2 = pElem->Attribute("EVENT_CLIENT");
			}
			else
			{
				// ERROR ????
			}
			pElem=hRoot.FirstChild("KALMAN_FILTER").Element();
			if (pElem)
			{
				// Read the 2-State Kalman parameters
				m_ProcessNoise                = atoi(pElem->Attribute("PROCESS_NOISE"));
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
	shmFileCamera->SetAttribute("FILE",         m_SHM_C3_CAMERA_TRACK.c_str());
	shmFileCamera->SetAttribute("MUTEX",		m_SHM_C3_CAMERA_TRACK_MUTEX.c_str());
	shmFileCamera->SetAttribute("EVENT_SERVER", m_SHM_C3_CAMERA_TRACK_EVENT1.c_str());
	shmFileCamera->SetAttribute("EVENT_CLIENT", m_SHM_C3_CAMERA_TRACK_EVENT2.c_str());

	TiXmlElement * shmFileLaser = new TiXmlElement( "SHM_FILE_LASER_TRACK" );  
	root->LinkEndChild( shmFileLaser );  
	shmFileLaser->SetAttribute("FILE",         m_SHM_C3_LASER_POINTING.c_str());
	shmFileLaser->SetAttribute("MUTEX",        m_SHM_C3_LASER_POINTING_MUTEX.c_str());
	shmFileLaser->SetAttribute("EVENT_SERVER", m_SHM_C3_LASER_POINTING_EVENT1.c_str());
	shmFileLaser->SetAttribute("EVENT_CLIENT", m_SHM_C3_LASER_POINTING_EVENT2.c_str());

	TiXmlComment * comment2 = new TiXmlComment();
	comment2->SetValue("Settings for the WENDE C3 2-State Kalman Filter");
	root->LinkEndChild( comment2 );    

	TiXmlElement * kalmanFilter = new TiXmlElement( "KALMAN_FILTER" );  
	root->LinkEndChild( kalmanFilter );  
	itoa(m_ProcessNoise,temp,10);
	kalmanFilter->SetAttribute("PROCESS_NOISE", temp);
	
	TiXmlComment * comment3= new TiXmlComment();
	comment3->SetValue("Settings for the WENDE C3 TRACK SETTINGS");
	root->LinkEndChild( comment3 );

	doc.SaveFile( m_CfgFile.c_str() ); 
}
