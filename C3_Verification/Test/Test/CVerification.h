#include "stdafx.h"
#include <string>
#include <vector>

using namespace std;


public ref class CVerification
{
public: CVerification(){};

public:
int Import_Test_Data_File(void);
string Get_Test_Data_Item(int nRunNumber, string sDataItem);

};
