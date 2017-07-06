// Logger_impl.h: interface for the Logger_impl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGER_IMPL_H__204F64E0_D755_4A5A_8AC1_19204D5AC0AB__INCLUDED_)
#define AFX_LOGGER_IMPL_H__204F64E0_D755_4A5A_8AC1_19204D5AC0AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32
#pragma warning (disable: 4786)
#endif

#include <vector>
#include "comLib/Logger.h"
#include "comLib/Mutex.h"

namespace comLib
{

class LogAppender
{
public:
	LogAppender();
	virtual ~LogAppender();

	virtual void Open(const char* pszTag)=0;
	virtual void AppendLine(const char* msg)=0;
};


class Logger_impl  
{
public:
	static void Open(const char* pszTag=0);	
	static int GetLogLevel();						//取得Tag的Level.
	static void SetLogLevel(int nLevel);			//设置Tag的Level.
	static void AppendLine(const std::string& msg);
private:
	static Mutex  m_lock;
	static int m_nLogLevel;
	static std::vector<LogAppender*>       m_appenders;
};


class LogAppenderConsole : public LogAppender  
{
public:
	void AppendLine(const char* msg);
	void Open(const char* pszTag);
	LogAppenderConsole();
	virtual ~LogAppenderConsole();

};

class LogAppenderFile : public LogAppender  
{
public:
	void AppendLine(const char* msg);
	void Open(const char* pszTag);
	LogAppenderFile();
	virtual ~LogAppenderFile();

	void Close();
	static void SetLogFilePath(const char* pszDir);
	static void SetLogFileMaxLine(int MaxLine);
private:
	Mutex  m_lock;
	std::ofstream	m_cLogFile;
	std::string	    m_cszLogFileFullPath;
	int			m_nTotalLogTimes;
	
	static std::string	    m_cszLogFileFullPathTag;
	static int m_MaxFileLine;
	void Open();
	void NewLog();
	std::string GetFileName();
};


class LogAppenderShareMem : public LogAppender  
{
public:
	void AppendLine(const char* msg);
	void Open(const char* pszTag);
	LogAppenderShareMem();
	virtual ~LogAppenderShareMem();
};

} //namespace comLib

#endif // !defined(AFX_LOGGER_IMPL_H__204F64E0_D755_4A5A_8AC1_19204D5AC0AB__INCLUDED_)
