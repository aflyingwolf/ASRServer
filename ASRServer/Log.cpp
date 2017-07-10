#include "stdafx.h"
#include "Log.h"
#include "Help.h"
#include <sys/timeb.h>

ILog::ILog()
{
}
ILog::~ILog()
{
}

Log::Log(char * logPath,char * logPrevName)
{
	strcpy(this->_logPath,logPath);
	strcpy(this->_logPrevName,logPrevName);
}

Log::Log()
{
}
Log::~Log()
{
}
void Log::WriteLog(LogLevel level,string log)
{
	WriteLog(level,log.c_str());
}
void Log::WriteLog(LogLevel level,const char * stringp, ...)
{
	va_list ap;
    FILE *fout = NULL;
    char filebuf[80], str[2560];
    char datebuf [9];
    struct _timeb timebuffer;
    char  timeline[50];
	char strLogLevel[50];
	try{
		memset(str,0,sizeof(str));
		memset(timeline,0,sizeof(timeline));
		memset(strLogLevel,0,sizeof(strLogLevel));
		if(level==MESS_INFO)
			strcpy(strLogLevel,"MESS_INFO");
		else if(level==DEBUG_INFO)
			strcpy(strLogLevel,"DEBUG_INFO");
		else if(level==ERROR_INFO)
			strcpy(strLogLevel,"ERROR_INFO");
		else
			strcpy(strLogLevel,"OTHER_INFO");
		char timeTemp[50];
		memset(timeTemp,0,50);
		Help::getcurrenttime("%Y%m%d",timeTemp,50);
		sprintf(filebuf,"%s\\%s%s.log",this->_logPath,this->_logPrevName,timeTemp);
		fout = fopen(filebuf, "at+");
		if (fout!=NULL) {
			va_start(ap,stringp);
				 _strdate( datebuf );
				 _ftime( &timebuffer );
				 sprintf(timeline, "%.9s.%hu", ctime(&(timebuffer.time))+10, timebuffer.millitm);
				 sprintf(str, "%s: %s %s ",strLogLevel,datebuf, timeline);
				 fputs(str,fout);
				 vfprintf(fout, stringp, ap);
				 fputs("\n",fout);
				 fclose(fout); fout=NULL;
			va_end(ap);
		}
    }
	catch(...){
		TRACE("WriteLog Error");
	}
	return;
}