#pragma once
#include "stdafx.h"
#include <windows.h>

#define C3_NETWORK_THREAD_COUNT 6

typedef struct{
	HANDLE handle;
	UINT   uiThreadId;
}NETWORK_THREAD_STRUCT;

typedef UINT (WINAPI *ptFunction)(LPVOID);

typedef struct
{
	bool statusRecv;
	bool trackRecv;
}NetworkStatusLink;

class CNetworkMonitor
{
	public:
		CNetworkMonitor();
		~CNetworkMonitor();

		void InitializeThread();
		void StopThreads();
		void StartCalibration();

	public:
		bool _isRunning;

		CRITICAL_SECTION cs; /* This is the critical section object -- once initialized,
			    			    it cannot be moved in memory */
			                 /* If you program in OOP, declare this as a non-static member in your class */
		CRITICAL_SECTION statusLock; /* This is the critical section object -- once initialized,
			    			    it cannot be moved in memory */
			                 /* If you program in OOP, declare this as a non-static member in your class */

		NetworkStatusLink stLink;
	public:
		NETWORK_THREAD_STRUCT threads[C3_NETWORK_THREAD_COUNT];
		ptFunction functionArray[C3_NETWORK_THREAD_COUNT];
};