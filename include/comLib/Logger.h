// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_H__0C0D5FA0_DAC4_4BB8_B2A7_EFD14C996639__INCLUDED_)
#define AFX_LOGGER_H__0C0D5FA0_DAC4_4BB8_B2A7_EFD14C996639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <string>
#include <vector>
#include <fstream>

#include "comLib/comdefine.h"
#include "comLib/Mutex.h"

namespace comLib
{

class Logger;
class LogManager  
{
public:
	//log example //2008-08-08 08:08:08.888-DG-[threadid][Tag][str1][str2][str3]this is log-test-message.
	///////////////////////////////////////////////////////////
	static void Open(const char* pszTag=0);	//初始化. filename prefix, default is 'log'
	static int GetLogLevel();						//取得Tag的Level.
	static void SetLogLevel(int nLevel);			//设置Tag的Level.
	static void SetLogFilePath(const char* pszDir);
	static void SetLogFileMaxLine(int maxLine);
	static bool CreateLongDirectory(const std::string& strDirectory);
};

class LogLayout
{
public:
	LogLayout();
	LogLayout(char* str1);
	LogLayout(char* str1, char* str2);
	LogLayout(char* str1, char* str2, char* str3);
	void AppendLine(const char* format, ...);
private:
	std::string NowStringNoYear();
	std::string GetLevelString(int nLevel2Str);
	std::string GetLayoutString(int nLevel2Str, std::string strClassTag);//time-LV-[threadid][Tag][str1][str2][str3]
	std::string m_strBuf;//[str1][str2][str3]
	//std::string m_strMessage;	//message from format
	std::vector<std::string>       m_messages;
	friend class Logger;
};

// log interface
class Logger  
{
public:
	enum	LOG_LEVEL
	{
		LL_FATAL	= 0,		//fatal	error
		LL_ERROR	=1,     	//error
		LL_WARN		=3,		//warning
		LL_TRACE	=5,		//trace
		LL_INFO		=7,		//info
		LL_DEBUG	=9,		//debug
		LL_ALL = 99
	};
public:
	Logger();
	Logger(char* pszTag);
	virtual ~Logger();
	void Debug(const char* format, ...);    //9
	void Info(const char* format, ...);    //7
	void Trace(const char* format, ...);    //5
	void Warn(const char* format, ...);    //3
	void Error(const char* format, ...);  //  1
	void Fatal(const char* format, ...);  //Level 0
	void Write(const char* format, ...);  //Level -1 , noLevel
	void Append(const char* msg);			//Level -1 , noLevel
	bool isDebugEnabled();
	bool isInfoEnabled();
	bool isTraceEnabled();
	bool isWarnEnabled();
	bool isErrorEnabled();
	bool isFatalEnabled();
	void Debug(LogLayout& lot );    //9
	void Info(LogLayout& lot);    //7
	void Trace(LogLayout& lot);    //5
	void Warn(LogLayout& lot);    //3
	void Error(LogLayout& lot);  //  1
	void Fatal(LogLayout& lot);  //Level 0
	void Write(LogLayout& lot);  //Level -1 , noLevel
	//void SetTag(const char* pszTag);
	void SetTag(const char* format, ...);
private:
	char m_strTag[32];
	friend class LogManager;
};


} //namespace comLib

#endif // !defined(AFX_LOGGER_H__0C0D5FA0_DAC4_4BB8_B2A7_EFD14C996639__INCLUDED_)
