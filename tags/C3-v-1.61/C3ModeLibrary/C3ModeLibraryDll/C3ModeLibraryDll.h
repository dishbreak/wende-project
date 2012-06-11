// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the C3MODELIBRARYDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// C3MODELIBRARYDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef C3MODELIBRARYDLL_EXPORTS
#define C3MODELIBRARYDLL_API __declspec(dllexport)
#else
#define C3MODELIBRARYDLL_API __declspec(dllimport)
#endif

// This class is exported from the C3ModeLibraryDll.dll
class C3MODELIBRARYDLL_API CC3ModeLibraryDll {
public:
	CC3ModeLibraryDll(void);
	// TODO: add your methods here.
};

extern C3MODELIBRARYDLL_API int nC3ModeLibraryDll;

C3MODELIBRARYDLL_API int fnC3ModeLibraryDll(void);
