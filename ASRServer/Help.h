#pragma once
class Help
{
public:
	Help(void);
	~Help(void);
public:
	static bool getcurrenttime(char * pFormatType,char * pTime,int nLen);
	//取字串
	static int Substr(char* dest,char* src,int post,char split);
	//检查是否是号码,仅判断号码长度至少6为
	static bool isPhone(const char * str);
	static int copyfile(char* src,char* dest);
};

