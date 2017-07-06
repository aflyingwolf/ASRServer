// SysUtil.cpp: implementation of the SysUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "comLib/comdefine.h"
#include "comLib/SysUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace comLib
{
bool SysUtil::CreateLongDirectory(const std::string& strDirectory)
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
		_mkdir(sNew.c_str());    // CreateDirectory(sNew.c_str() ,NULL);
#else
        mkdir(sNew.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	}
	return true;
}

void SysUtil::SleepMillisecond(unsigned long millisecond)
{
#ifdef WIN32
	Sleep(millisecond);
#else
#ifndef _AIX
    struct timeval tm;    
    tm.tv_sec = 0; 
    tm.tv_usec = millisecond * 1000; //—” ±∫¡√Î 
    select(0,NULL,NULL,NULL,&tm);
#else
	usleep(millisecond * 1000);
#endif
#endif
}

void SysUtil::SleepSecond(unsigned long second)
{
#ifdef WIN32
	Sleep(second * 1000);
#else
#ifndef _AIX
    struct timeval tm;    
    tm.tv_sec = second; 
    tm.tv_usec = 0; //—” ±∫¡√Î 
    select(0,NULL,NULL,NULL,&tm);
#else
	sleep(second);
#endif
#endif
}

} //namespace comLib
