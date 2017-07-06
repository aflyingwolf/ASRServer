// Logger.cpp: implementation of the Logger class.
//
//////////////////////////////////////////////////////////////////////

#include "comLib/Logger.h"
#include "comLib/Logger_impl.h"

#include <stdio.h>
#include <stdarg.h>
#include <sys/timeb.h>

#ifdef WIN32
#   include <time.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace comLib
{

#ifndef _WINDOWS_
#define _vsnprintf(s1, n, s2, v)  vsnprintf(s1, n, s2, v)
#endif

Logger::Logger()
{
	memset(m_strTag, 0, sizeof(m_strTag));
}

Logger::Logger(char* pszTag)
{
	memset(m_strTag, 0, sizeof(m_strTag));
	strncpy(m_strTag, pszTag, sizeof(m_strTag)-1);
}

Logger::~Logger()
{

}
void Logger::SetTag(const char* format, ...)
{
	memset(m_strTag, 0, sizeof(m_strTag));	
	try
	{
		va_list args;
		va_start(args, format);		
		int nBuf = _vsnprintf(m_strTag, sizeof(m_strTag)-1, format, args);
		va_end(args);
	}catch(...){}
}


#define GENERAL_LOG_FORMAT(X)	 \
	LogLayout lot;				\
	std::string buf = lot.GetLayoutString(X, m_strTag);  \
	int nBuf;													\
	char szBuffer[512];										\
	memset(szBuffer, 0, sizeof(szBuffer));					\
	try{												\
		va_list args;									\
		va_start(args, format);							\
		nBuf = _vsnprintf(szBuffer, 512, format, args);	\
		va_end(args);									\
	}catch(...){}										\
	szBuffer[511]=0;									\
	buf.append(szBuffer);								\
	try{												\
		Logger_impl::AppendLine(buf);					\
	}catch(...){}				

void Logger::Debug(const char* format, ...)
{
	if (false == isDebugEnabled()) return;
	//组串
	LogLayout lot;
	std::string buf = lot.GetLayoutString(LL_DEBUG, m_strTag);
//	std::string buf;
	int nBuf;
	char szBuffer[512] = {0};

	try
	{
		va_list args;
		va_start(args, format);

#ifdef _WINDOWS_
		nBuf = _vsnprintf(szBuffer, 512, format, args);
#else		// no windows
		nBuf = vsnprintf(szBuffer, 512, format, args);
#endif
		va_end(args);
	}catch(...){}
	szBuffer[511]=0;
	buf.append(szBuffer);
	try
	{
		Logger_impl::AppendLine(buf);
	}catch(...){}
}
void Logger::Info(const char* format, ...)
{
	if (false == isInfoEnabled()) return;
	GENERAL_LOG_FORMAT(LL_INFO);
}

void Logger::Trace(const char* format, ...)
{
	if (false == isTraceEnabled()) return;
	GENERAL_LOG_FORMAT(LL_TRACE);
}

void Logger::Warn(const char* format, ...)
{
	if (false == isWarnEnabled()) return;
	GENERAL_LOG_FORMAT(LL_WARN);
}

void Logger::Error(const char* format, ...)
{
	if (false == isErrorEnabled()) return;
	GENERAL_LOG_FORMAT(LL_ERROR);
}

void Logger::Fatal(const char* format, ...)
{
	if (false == isFatalEnabled()) return;
	GENERAL_LOG_FORMAT(LL_FATAL);
}

void Logger::Write(const char* format, ...)
{
	GENERAL_LOG_FORMAT(LL_INFO);
}

void Logger::Append(const char* msg)
{
	if (false == isDebugEnabled()) return;
	//组串
	LogLayout lot;
	std::string buf = lot.GetLayoutString(LL_DEBUG, m_strTag);
	buf.append(msg);

	Logger_impl::AppendLine(buf);

}

bool Logger::isDebugEnabled()
{
	return (Logger_impl::GetLogLevel()>=LL_DEBUG);
}

bool Logger::isInfoEnabled()
{
	return (Logger_impl::GetLogLevel()>=LL_INFO);
}

bool Logger::isTraceEnabled()
{
	return (Logger_impl::GetLogLevel()>=LL_TRACE);
}

bool Logger::isWarnEnabled()
{
	return (Logger_impl::GetLogLevel()>=LL_WARN);
}

bool Logger::isErrorEnabled()
{
	return (Logger_impl::GetLogLevel()>=LL_ERROR);
}

bool Logger::isFatalEnabled()
{
	return (Logger_impl::GetLogLevel()>=LL_FATAL);
}

void Logger::Debug(LogLayout& lot )
{
	if (false == isDebugEnabled()) return;
	//组串
	std::string buf = lot.GetLayoutString(LL_DEBUG, m_strTag);
	Logger_impl::AppendLine(buf);

}

void Logger::Info(LogLayout& lot)
{
	if (false == isInfoEnabled()) return;
	//组串
	std::string buf = lot.GetLayoutString(LL_INFO, m_strTag);
	Logger_impl::AppendLine(buf);
}

void Logger::Trace(LogLayout& lot)
{
	if (false == isTraceEnabled()) return;
	//组串
	std::string buf = lot.GetLayoutString(LL_TRACE, m_strTag);
	Logger_impl::AppendLine(buf);
}

void Logger::Warn(LogLayout& lot)
{
	if (false == isWarnEnabled()) return;
	//组串
	std::string buf = lot.GetLayoutString(LL_WARN, m_strTag);
	Logger_impl::AppendLine(buf);
}

void Logger::Error(LogLayout& lot)
{
	if (false == isErrorEnabled()) return;
	//组串
	std::string buf = lot.GetLayoutString(LL_ERROR, m_strTag);
	Logger_impl::AppendLine(buf);
}

void Logger::Fatal(LogLayout& lot)
{
	if (false == isFatalEnabled()) return;
	//组串
	std::string buf = lot.GetLayoutString(LL_FATAL, m_strTag);
	Logger_impl::AppendLine(buf);
}

void Logger::Write(LogLayout& lot)
{
	//组串
	std::string buf = lot.GetLayoutString(LL_ALL, m_strTag);
	Logger_impl::AppendLine(buf);
}


std::string LogLayout::GetLevelString(int nLevel2Str)
{
	switch(nLevel2Str)
	{
	case Logger::LL_FATAL: return "FR";
	case Logger::LL_ERROR: return "ER";
	case Logger::LL_WARN:  return "WA";
	case Logger::LL_TRACE: return "TR";
	case Logger::LL_INFO:  return "IF";
	case Logger::LL_DEBUG: return "DT";
	case Logger::LL_ALL: return "**";
	default: return "--";
	}
}
LogLayout::LogLayout()
{
	m_strBuf="";
}
LogLayout::LogLayout(char* str1)
{
	m_strBuf.append("[").append(str1).append("][][]");
}
LogLayout::LogLayout(char* str1, char* str2)
{
	m_strBuf.append("[").append(str1).append("]");
	m_strBuf.append("[").append(str2).append("][]");
}
LogLayout::LogLayout(char* str1, char* str2, char* str3)
{
	m_strBuf.append("[").append(str1).append("]");
	m_strBuf.append("[").append(str2).append("]");
	m_strBuf.append("[").append(str3).append("]");
}
void LogLayout::AppendLine(const char* format, ...)
{
	int nBuf;
	char szBuffer[512];
	memset(szBuffer, 0, sizeof(szBuffer));
	
	try
	{
		va_list args;
		va_start(args, format);
		
#ifdef _WINDOWS_
		nBuf = _vsnprintf(szBuffer, 512, format, args);
#else		// no windows
		nBuf = vsnprintf(szBuffer, 512, format, args);
#endif
		va_end(args);
	}catch(...){}
	szBuffer[511]=0;
	//m_strMessage.append(szBuffer);
	m_messages.push_back(szBuffer);
}

//time-LV-[threadid][Tag][str1][str2][str3]
std::string  LogLayout::GetLayoutString(int nLevel2Str, std::string strClassTag)
{
	char   strFix[256];
	memset(strFix, 0, sizeof(strFix));
	//GetCurrentThreadID();
#ifdef WIN32
	DWORD tid = GetCurrentThreadId();
#else
	pthread_t stid = pthread_self();
	DWORD tid = stid;
#ifndef _AIX
//	tid = tid<<8;
//	tid = tid>>20;
#endif //_AIX

#endif
	if ( m_strBuf.size()<=0 )
	{
		sprintf(strFix, "[%s][%s]-[%u][%s]",
			GetLevelString(nLevel2Str).c_str(), NowStringNoYear().c_str(), 
				tid, strClassTag.c_str());
	}else{
		sprintf(strFix, "[%s][%s]-[%u][%s]%s",
				GetLevelString(nLevel2Str).c_str(), NowStringNoYear().c_str(), 
				tid, strClassTag.c_str(), m_strBuf.c_str());
	}
	std::string buf;
	
	if ( m_messages.size()<=0)
	{
		buf.append(strFix);
	}
	else
	{
		typedef std::vector<std::string>::iterator itVector;
		for (itVector iter = m_messages.begin(); iter != m_messages.end(); ++iter) 
		{ 
			buf.append(strFix);
			buf.append(*iter);
			buf.append("\n");
		} 
		buf.erase(buf.size()-2);
	}

	return buf;
}

std::string  LogLayout::NowStringNoYear()
{
 	struct tm* tmTemp;
#ifdef WIN32
    struct _timeb tb;
    _ftime(&tb);
	tmTemp = localtime(&tb.time);
#else		// no windows
    struct timeb tb;
	ftime(&tb);
	struct tm tmTemp2;
	localtime_r(&tb.time, &tmTemp2);
	tmTemp = &tmTemp2;
#endif
	int tm_mon = (*tmTemp).tm_mon;
	int tm_mday = (*tmTemp).tm_mday;
	int tm_hour = (*tmTemp).tm_hour;
	int tm_min = (*tmTemp).tm_min;
	int tm_sec = (*tmTemp).tm_sec;
	int tm_millisec = tb.millitm;
	tm_mon += 1;			//[1,12]
	char szTime[32];
	sprintf(szTime,"%02d.%02d %02d:%02d:%02d.%03d",
		tm_mon, tm_mday, tm_hour, tm_min, tm_sec, tm_millisec);

	return szTime;
}

//////////////////////////////////////////////////////////////////////
// LogFactory Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//初始化. filename prefix
void LogManager::Open(const char* pszTag)	
{
	Logger_impl::Open(pszTag);
}

int LogManager::GetLogLevel()
{
	return Logger_impl::GetLogLevel();
}

void LogManager::SetLogLevel(int nLevel)
{
	Logger_impl::SetLogLevel(nLevel);
}

void LogManager::SetLogFileMaxLine(int maxLine)
{
	LogAppenderFile::SetLogFileMaxLine(maxLine);
}

void LogManager::SetLogFilePath(const char* pszDir)
{
	LogAppenderFile::SetLogFilePath(pszDir);
	if (0!= pszDir)
	{
		CreateLongDirectory(pszDir);
	}
}

bool LogManager::CreateLongDirectory(const std::string& strDirectory)
{
	int index1 = -1, index2 = -1;
	std::string sNew;
	std::string slashset="/\\";
	while(1)
	{
		index2 = index1 + 1;
		index1 = strDirectory.find_first_of(slashset,index2);
		if(index1 == std::string::npos)
			break;
		if(index1 == 0)
			continue;
		if(strDirectory[index1 - 1] == ':')
			continue;
		sNew = strDirectory.substr(0, index1);
#ifdef WIN32
		_mkdir(sNew.c_str());    
#else
        mkdir(sNew.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	}
	return true;
}




} //namespace comLib
