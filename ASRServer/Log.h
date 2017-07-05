#pragma once

class Log
{
public:
	Log(char * logPath,char * logPrevName);
	~Log(void);
private:
	char _logPath[256];
	char _logPrevName[256];
public:
	enum LogLevel{MESS_INFO=0,DEBUG_INFO,ERROR_INFO};
	//–¥»’÷æ
	void WriteLog(LogLevel level,const char * stringp, ...);
};

