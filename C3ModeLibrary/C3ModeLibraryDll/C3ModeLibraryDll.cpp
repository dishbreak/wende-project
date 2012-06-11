// C3ModeLibraryDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "C3ModeLibraryDll.h"


// This is an example of an exported variable
C3MODELIBRARYDLL_API int nC3ModeLibraryDll=0;

// This is an example of an exported function.
C3MODELIBRARYDLL_API int fnC3ModeLibraryDll(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see C3ModeLibraryDll.h for the class definition
CC3ModeLibraryDll::CC3ModeLibraryDll()
{
	return;
}
