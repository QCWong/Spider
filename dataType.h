#pragma once

const bool DEBUG = true;

const int URLSIZE = 1024;
const int HOSTSIZE = 1024;
const int DIRSIZE = 1024;
const int FILESIZE=1024;
const int REQUESTSIZE = 1024;
const char SAVEDIR[] = "XML/";
const int SAVEFILESIZE = 1024;

const char PROTOCAL_VER[] = "HTTP/1.1";
const char ACCEPT[] = "*/*";
const char USER_AGENT[] = "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)";
const char CONNECTION[] = "Keep-Alive";
#define CONTENTTYPE "application/x-www-form-urlencoded"
#define ACCEPTLANGUAGE "zh-cn,zh;q=0.5"
#define ACCEPTENCODING "gzip,deflate"
#define ACCEPTCHARSET "gb2312,utf-8;q=0.7,*;q=0.7"
#define KEEPALIVE "300"