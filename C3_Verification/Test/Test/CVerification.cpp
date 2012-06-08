#include "stdafx.h"
#include "CVerification.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct stTestRun
{
	string nInRoverX;
	string nInRoverY;
	string nOutRoverX;
	string nOutRoverY;
};

// Initialise vector to 1000 entries
std::vector<stTestRun> vTestRun(1000);

int CVerification::Import_Test_Data_File(void)
{
	string line;
	int i = 0;
	ifstream myfile ("C3_Test_Data.txt");
	if (myfile.is_open())
	{
		while (myfile.good())
		{
			// Character positions
			size_t found;

			// Line contains rover input and output x and y
			getline(myfile, line);

			// Following lines of code progressively reduce original string
			found = line.find_last_of(",");
			string sOutRoverY = line.substr(found+1);
			string temp1 = line.substr(0,found);
			
			found = temp1.find_last_of(",");
			string sOutRoverX = temp1.substr(found+1);
			string temp2 = temp1.substr(0,found);

			found = temp2.find_last_of(",");
			string sInRoverY = temp2.substr(found+1);
			string temp3 = temp2.substr(0,found);

			found = temp3.find_last_of(",");
			string sInRoverX = temp3.substr(found+1);

			// For a particular element populate in and out x and y
			vTestRun.at(i).nInRoverX = sInRoverX;
			vTestRun.at(i).nInRoverY = sInRoverY;
			vTestRun.at(i).nOutRoverX = sOutRoverX;
			vTestRun.at(i).nOutRoverY = sOutRoverY;
			i++;
		}
		myfile.close();
	}
	return 0;
}

string CVerification::Get_Test_Data_Item(int nRunNumber, string sDataItem)
{
	string value;

	if(sDataItem == "RoverInX")
		value = vTestRun.at(nRunNumber).nInRoverX;
	if(sDataItem == "RoverInY")
		value = vTestRun.at(nRunNumber).nInRoverY;
	if(sDataItem == "RoverOutX")
		value = vTestRun.at(nRunNumber).nOutRoverX;		
	if(sDataItem == "RoverOutY")
		value = vTestRun.at(nRunNumber).nOutRoverY;

	return value;
}
