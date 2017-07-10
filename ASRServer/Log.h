#pragma once

#include <string>
using namespace std;

class ILog
{
public:
	ILog();
	~ILog();
public:
	enum LogLevel{MESS_INFO=0,DEBUG_INFO,ERROR_INFO};
	virtual void WriteLog(LogLevel level,string log)=0;
};

class Log:public ILog
{
public:
	Log();
	Log(char * logPath,char * logPrevName);
	~Log();
private:
	char _logPath[256];
	char _logPrevName[256];
public:
	//–¥»’÷æ
	void WriteLog(LogLevel level,const char * stringp, ...);
	virtual void WriteLog(LogLevel level,string log);
};

