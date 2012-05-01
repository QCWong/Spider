#include <iostream>

#include "spider.h"
using namespace std;

int main()
{
	char src[URLSIZE] = "http://www.baidu.com/";
	Spider spider;
	spider.initSpider(src);
	spider.run();

	return 0;
	
}