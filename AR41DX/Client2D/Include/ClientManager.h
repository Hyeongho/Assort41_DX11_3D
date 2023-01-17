#pragma once

#include "Client.h"

class CClientManager
{
public:
	CClientManager();
	~CClientManager();

public:
	bool Init(HINSTANCE hInst);
	int Run();
};

