#pragma once
#include "stdafx.h"
#include <windows.h>

class CNetworkMonitor
{
public:
	CNetworkMonitor();
	~CNetworkMonitor();

	void InitializeThread();
	void StopThreads();

public:
	bool _isRunning;

	CRITICAL_SECTION cs; /* This is the critical section object -- once initialized,
					    it cannot be moved in memory */
			         /* If you program in OOP, declare this as a non-static member in your class */

};