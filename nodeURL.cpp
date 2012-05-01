#include "nodeURL.h"
#include "dataType.h"

NodeURL::NodeURL(void)
{
	memset(host, '\0', sizeof(char) * HOSTSIZE);
	memset(file, '\0', sizeof(char)* FILESIZE);
	memset(dir, '\0', sizeof(char)* DIRSIZE);
	memset(saveAddr, '\0', sizeof(char) * SAVEFILESIZE);
	port = 0;
	isHandled = false;
	sockfd = 0;
}


NodeURL::~NodeURL(void)
{
}
