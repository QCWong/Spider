#pragma once
#include "dataType.h"
#include <string.h>
#include <stdlib.h>

class NodeURL
{
public:
	NodeURL(void);
	virtual ~NodeURL(void);

public:
	char host[HOSTSIZE];
	int port;
	char dir[DIRSIZE];
	char file[FILESIZE];
	bool isHandled;
	char saveAddr[FILESIZE];	
	int sockfd;
};

