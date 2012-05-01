#pragma once

#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <locale.h>
#include <iostream>
#include <fstream>

#include "dataType.h"
#include "nodeURL.h"
using namespace std;


class Spider
{
public:
	Spider(void);
	virtual ~Spider(void);

public:
	int initSpider(char *src);
	void run();
	int handleUrlVector();
	int handleOneUrl( NodeURL * nodeUrl);
	int getInfoFromURL(char *url, NodeURL *nodeUrl);
	int connectWebSite(NodeURL *nodeUrl);
	int fillGETpkg(const NodeURL *nodeUrl, char *requestline);
	int sendRequest(const NodeURL *nodeUrl);
	int getResult(const NodeURL *nodeUrl);
	int getUrl( char *src, char *url );
	int AnalysisXML(const NodeURL *nodeUrl);

public:
	char *sourceURL;
	vector <NodeURL> urlVector;
	int fileRandeNum;
	
};

