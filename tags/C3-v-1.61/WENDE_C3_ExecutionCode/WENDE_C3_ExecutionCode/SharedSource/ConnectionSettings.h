#pragma once

#include <string>

using std::string;

class ConnectionSettings
{
public:
	ConnectionSettings(void);
	~ConnectionSettings(void);

public:
	string ip;
	int port;
};
