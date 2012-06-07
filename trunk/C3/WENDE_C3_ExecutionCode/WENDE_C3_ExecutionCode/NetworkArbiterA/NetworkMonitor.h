#pragma once
#include "tcpip/SocketManager.h"

class CNetworkMonitor
{
public:
	CNetworkMonitor();
	~CNetworkMonitor();

	void InitializeThread(CSocketManager* socketObjectServer);
	void StopThreads();

public:
	bool _isRunning;
	CSocketManager  *m_SocketObjectServer;
	CRITICAL_SECTION cs; /* This is the critical section object -- once initialized,
					    it cannot be moved in memory */
			         /* If you program in OOP, declare this as a non-static member in your class */

};