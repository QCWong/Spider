#include <iostream>

#include "spider.h"
using namespace std;

int main()
{
	char src[URLSIZE];
	memset(src, 0, sizeof(src));
	memcpy(src, "http://www.baidu.com/", strlen("http://www.baidu.com/"));
	Spider spider;
	spider.initSpider(src);
	spider.run();

	return 0;
	
}