// Logger_impl.cpp: implementation of the Logger_impl class.
//
//////////////////////////////////////////////////////////////////////

#include "comLib/Logger_impl.h"

#include <algorithm>
#include <sys/timeb.h>

#ifdef WIN32
#   include <time.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace comLib
{
int		Logger_impl::m_nLogLevel=11;
std::vector<LogAppender*>       Logger_impl::m_appenders;
Mutex  Logger_impl::m_lock;

void Logger_impl::Open(const char* pszTag)
{
	static LogAppenderConsole		lac;
	static LogAppenderFile			laf;
	static LogAppenderShareMem		lasm;
	m_appenders.push_back(&lac);
	m_appenders.push_back(&lasm);
	m_appenders.push_back(&laf);
	
	lac.Open(pszTag);
	lasm.Open(pszTag);
	laf.Open(pszTag);

    //std::for_each(m_appenders.begin(), m_appenders.end(), ???);
}
int Logger_impl::GetLogLevel()
{
	return m_nLogLevel;
}
void Logger_impl::SetLogLevel(int nLevel)
{
	m_nLogLevel = nLevel;
}
void Logger_impl::AppendLine(const std::string& msg)
{
	typedef std::vector<LogAppender*>::iterator itLog;

	SingleMutex s(m_lock);
	for (itLog iter = m_appenders.begin(); iter != m_appenders.end(); ++iter) 
	{ 
		LogAppender* pla = *iter;
		pla->AppendLine(msg.c_str());
	} 
}


//////////////////////////////////////////////////////////////////////
// LogAppenderConsole Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LogAppender::LogAppender()
{

}

LogAppender::~LogAppender()
{

}

LogAppenderConsole::LogAppenderConsole()
{

}

LogAppenderConsole::~LogAppenderConsole()
{

}


//////////////////////////////////////////////////////////////////////
// LogAppenderFile Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef WIN32
std::string	    LogAppenderFile::m_cszLogFileFullPathTag=".\\" ;
#else
std::string	    LogAppenderFile::m_cszLogFileFullPathTag="./" ;
#endif
int LogAppenderFile::m_MaxFileLine=100*1024;
//int LogAppenderFile::m_MaxFileLine=100;  //”√”⁄≤‚ ‘

LogAppenderFile::LogAppenderFile()
{
}

LogAppenderFile::~LogAppenderFile()
{
	Close();
}

void LogAppenderFile::SetLogFileMaxLine(int MaxLine)
{
	if (MaxLine<100) return;
	m_MaxFileLine = MaxLine;
}

void LogAppenderFile::SetLogFilePath(const char* pszDir)
{
	if (0!= pszDir)
	{
		m_cszLogFileFullPathTag = pszDir;
		char slash;
#ifdef WIN32
		slash = '\\';
		if ( m_cszLogFileFullPathTag[m_cszLogFileFullPathTag.size()-1]  != slash )
			m_cszLogFileFullPathTag.append("\\");
#else
		slash = '/';
		if ( m_cszLogFileFullPathTag[m_cszLogFileFullPathTag.size()-1]  != slash )
			m_cszLogFileFullPathTag.append("/");
#endif
	}
}

void LogAppenderFile::Open()
{
	m_cszLogFileFullPath = GetFileName();
	m_cLogFile.open(m_cszLogFileFullPath.c_str(), std::ios::out);
}

std::string LogAppenderFile::GetFileName()
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
	tm_mon += 1;			//[1,12]
	char szTime[32];
	sprintf(szTime,"%02d.%02d.%02d-%02d-%02d",
		tm_mon, tm_mday, tm_hour, tm_min, tm_sec);

	std::string cszFileName ;
	cszFileName = m_cszLogFileFullPathTag;
	cszFileName += ".";
	cszFileName += szTime;
	cszFileName +=  ".log";
	
	return cszFileName;
}

void LogAppenderFile::Close()
{
	m_cLogFile.clear();		//open() never clears any state flags
	m_cLogFile.close();
//	CLogManager::Instance()->BackupFile(m_cszLogFileFullPath.c_str() );
}

void LogAppenderFile::NewLog()
{
    //LOCK(&m_FileLock);
	Close();
	Open();
	//UNLOCK(&m_FileLock);
}


//////////////////////////////////////////////////////////////////////
// LogAppenderShareMem Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogAppenderShareMem::LogAppenderShareMem()
{
}

LogAppenderShareMem::~LogAppenderShareMem()
{

}
void LogAppenderConsole::Open(const char *pszTag)
{

}
void LogAppenderFile::Open(const char *pszTag)
{
	static bool bInit = false;
	if ( true == bInit )
		return ;

	if (0!=pszTag)
		m_cszLogFileFullPathTag.append(pszTag);
	else
		m_cszLogFileFullPathTag.append("log");	// default tag(file prefix)

	Open();
}


void LogAppenderShareMem::Open(const char *pszTag)
{

}

void LogAppenderConsole::AppendLine(const char* msg)
{
	printf("%s\n",msg);
}
void LogAppenderFile::AppendLine(const char* msg)
{
	SingleMutex s(m_lock);
	try
	{
		m_nTotalLogTimes++;
		if(m_nTotalLogTimes >= m_MaxFileLine )
		{
			NewLog();
			m_nTotalLogTimes = 0;
		}
		
		if (m_cLogFile.is_open() == false) return;

		m_cLogFile << msg << std::endl;// << std::flush;
	}catch(...){}
}
void LogAppenderShareMem::AppendLine(const char* msg)
{

}
} //namespace comLib

