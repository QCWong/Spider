#include "spider.h"


Spider::Spider(void)
{
}


Spider::~Spider(void)
{
}


int Spider::initSpider( char *src )
{
	urlVector.clear();
	fileRandeNum = 1;
	memcpy(sourceURL, src, URLSIZE);
	NodeURL *root = new NodeURL();
	getInfoFromURL(sourceURL, root);
	urlVector.push_back(*root);
}

void Spider::run()
{
	handleUrlVector();
}

int Spider::handleUrlVector()
{
	int size = urlVector.size();
	NodeURL * nodeUrl;
	vector <NodeURL>::iterator it = urlVector.begin();
	for (; it != urlVector.end(); it++)
	{
		nodeUrl = &(*it);
		handleOneUrl(nodeUrl);
	}

	return 0;
}

int Spider::handleOneUrl( NodeURL * nodeUrl )
{
	connectWebSite(nodeUrl);

	sendRequest(nodeUrl);

	getResult(nodeUrl);

	AnalysisXML(nodeUrl);

	nodeUrl->isHandled = true;

	return 0;
}

/**************************************************************
功能：在字符串 s 里从末尾向前 搜索 x 字符，并设置指针 d 指向该位置
***************************************************************/
void Rstrchr(char * s, int x, char ** d)
{
	int len = strlen(s) - 1;
	while (len >= 0)	
	{
		if (x == s[len]) 
		{
			(*d) = s + len; 
			return;
		}
		len--;
	}
	(*d) = 0;
}

int Spider::getInfoFromURL( char *src, NodeURL *nodeUrl )
{

	int len;
	char *pHost, *pDir, *pFile, *pPort;

	if (!(*src))  
		return -1;
	
	pHost = src;
	if (!strncmp(pHost, "http://", strlen("http://")))  
		pHost = src + strlen("http://");
	else return 1;

	pDir = strchr(pHost, '/');

	if (pDir)  
	{
		len = strlen(pHost) - strlen(pDir);
		memcpy (nodeUrl->host, pHost, len);

		if (*(pDir + 1)) 
		{
			Rstrchr(pDir + 1, '/', &pFile);
			if (pFile)
				len = strlen(pDir + 1) - strlen(pFile);
			else 
				len = 0;

			if (len > 0) 
			{
				memcpy(nodeUrl->dir, pDir + 1, len);
				if (pFile + 1) 
				{
					len = strlen(pFile + 1);
					memcpy(nodeUrl->file, pFile + 1, len);
				}
				else 
				{
					len = 1;					
					memcpy(nodeUrl->file, "@", len);
				}
			}
			else 
			{
				len = 1;
				memcpy(nodeUrl->dir, "/", len);
				len = strlen(pDir + 1);
				memcpy(nodeUrl->file, pDir + 1, len);
			}
		}
		else
		{
			len = 1;
			memcpy(nodeUrl->dir, "/", len);
			len = 1;
			memcpy(nodeUrl->file, "@", len);
		}
	}
	else  
	{
		len = strlen(pHost);
		memcpy(nodeUrl->host, pHost, strlen(pHost));
		len = 1;
		memcpy(nodeUrl->dir, "/", len);
		len = 1;
		memcpy(nodeUrl->file, "@", len);
	}

	pPort = strchr(nodeUrl->host, ':');
	if (pPort)  
		nodeUrl->port = atoi(pPort + 1);
	else
		nodeUrl->port = 80;

	//itoa(fileRandeNum, nodeUrl->saveAddr, 10);
	sprintf(nodeUrl->saveAddr, "%d\0", fileRandeNum);
	fprintf(stdout, "fileRandeNum = %d, nodeUrl->saveAddr = %s", fileRandeNum, nodeUrl->saveAddr);
	fileRandeNum++;

	cout << "#######################start##################################"<<endl;
	cout << "src = " << src << endl;
	cout << "char host[] = \"" << nodeUrl->host <<"\";"<< endl;
	cout << "char file[] = \"" << nodeUrl->file <<"\";"<< endl;
	cout << "int port = " << nodeUrl->port <<";"<< endl;
	cout << "char dir[] = \"" << nodeUrl->dir <<"\";"<< endl;
	cout << "########################end#################################"<<endl;

	return 0;
}

int Spider::connectWebSite( NodeURL *nodeUrl )
{
	struct hostent *host;
	host = gethostbyname(nodeUrl->host);
	/* create a socket descriptor */
	int sockfd;
	if ((sockfd = socket(PF_INET,SOCK_STREAM,0)) == -1)
	{
		fprintf(stderr,"\tSocket Error:%s\a\n",strerror(errno));
		exit(1);
	}
	else
		nodeUrl->sockfd = sockfd;

	struct sockaddr_in server_addr;
	/* bind address */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(nodeUrl->port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	/* connect to the server */
	if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "\tConnect Error:%s\a\n", strerror(errno));
		exit(1);
		return 1;
	}
	else
	{
		if (DEBUG)
			fprintf(stdout, "\tConnect Accept!\n");
		return 0;
	}
}

int Spider::fillGETpkg(const NodeURL *nodeUrl, char *requestline)
{
	memset(requestline, '\0', sizeof(char)*REQUESTSIZE);
	if(strcmp(nodeUrl->dir, "/"))
	{
		sprintf(requestline, "GET /%s/%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: %s\r\nConnection: %s\r\n\r\n", nodeUrl->dir, strcmp(nodeUrl->file, "@")?nodeUrl->file:"", nodeUrl->host, USER_AGENT, ACCEPT, CONNECTION);
	}
	else
	{
		sprintf(requestline, "GET %s/%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: %s\r\nConnection: %s\r\n\r\n", nodeUrl->dir, strcmp(nodeUrl->file, "@")?nodeUrl->file:"", nodeUrl->host, USER_AGENT, ACCEPT, CONNECTION);
	}

	if (DEBUG)
	{
		cout<<"#######################GET PKG############"<<endl;
		cout<<requestline <<endl;
		cout<<"##########################################"<<endl;

	}
	//strcpy(requestline, request);
	return 0;
}

int Spider::sendRequest( const NodeURL *nodeUrl )
{
	char request[REQUESTSIZE];
	fillGETpkg(nodeUrl, request);

	int thisSend = 0;
	int totalSend = 0;
	int nbytes = strlen(request);
	while (totalSend < nbytes) 
	{
		thisSend = write(nodeUrl->sockfd, request + totalSend, nbytes - totalSend);
		if (thisSend == -1)
		{
			fprintf(stderr, "\tsend GET pkg error!%s\n", strerror(errno));
			exit(0);
		}
		totalSend += thisSend;
		fprintf(stdout, "\n\tRequest %d bytes send OK!\n", totalSend);

	}
}


int Spider::getResult( const NodeURL *nodeUrl )
{
	char saveFileName[SAVEFILESIZE];
	FILE *savefp = NULL;
	int nbytes = 0, i = 0, j = 0;
	char buffer[1024], httpHeader[200];

	strcpy(saveFileName, SAVEDIR);
	fprintf(stdout, "saveFileName = %s, SAVEDIR = %s\n", saveFileName, SAVEDIR);
	strcat(saveFileName, nodeUrl->saveAddr);
	fprintf(stdout, "saveFileName = %s, nodeUrl->saveAddr = %s\n", saveFileName, nodeUrl->saveAddr);


	if ((savefp = fopen(saveFileName, "w")) == NULL)
	{
		if(DEBUG)
			fprintf(stderr, "create file '%s' error\n", saveFileName);
		return 1;
	}
	else
	{
		if (DEBUG)
			fprintf(stdout, "open file %s ACCEPT!\n", saveFileName);
	}

	/* receive data from web server */
	memset(buffer, '\0', sizeof(buffer));
	while ((nbytes = read(nodeUrl->sockfd, buffer,1)) == 1)
	{
		if (i < 4)  
		{ 
			/* 获取 HTTP 消息头 */
			if (buffer[0] == '\r' || buffer[0] == '\n') 
				i++;
			else 
				i = 0;
			memcpy(httpHeader + j, buffer, 1); 
			j++;
		}
		else 
		{ 
			/* 获取 HTTP 消息体 */
			fprintf(savefp, "%c", buffer[0]); /* print content on the file */
			//fprintf(stdout, "%c", buffer[0]); /* print content on the screen */
			i++;
		}
	}
	fprintf(stdout, "XML has loaded.\n");
	fclose(savefp);
}

int Spider::getUrl( char *src, char *url )
{
	memset(url, '\0', sizeof(char) * URLSIZE);
	char *pHref, *pStart, *pEnd;
	int len;

	pHref = strstr(src, "href=\"");
	if (pHref)
	{
		pStart = src + 6;
		if (pStart)
		{
			pEnd = strchr(pStart, '\"');
			if (pEnd)
			{
				len = strlen(pEnd) - strlen(pStart);
				memcpy(url, pStart, len);
				
				return 1;
			}
			else return 0;

		}
		else return 0;
	}
	else
		return 0;
}

int Spider::AnalysisXML( const NodeURL *nodeUrl )
{
	char fileName[SAVEFILESIZE];
	FILE *readfp = NULL;
	int nbytes = 0, i = 0, j = 0;
	char buffer[1024], httpHeader[200];

	strcpy(fileName, SAVEDIR);
	strcat(fileName, nodeUrl->saveAddr);

	ifstream fin(fileName);
	char src[URLSIZE];
	char url[URLSIZE];
	while (fin.getline(src, URLSIZE))
	{
		if (getUrl(src, url) != 0)
		{
			if (strstr(url, "http://"))
			{
				NodeURL *nodeUrl = new NodeURL();
				getInfoFromURL(url, nodeUrl);
				urlVector.push_back(*nodeUrl);
			}
		}		
	}
}
