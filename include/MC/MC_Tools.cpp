#include "MC_Tools.h"
typedef unsigned char BYTE; 

void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)//TCHAR
//void Gb2312ToUnicode(TCHAR* pOut,char *gbBuffer)//TCHAR
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return;
}
void UnicodeToUTF_8(char* pOut,WCHAR* pText)
//void UnicodeToUTF_8(char* pOut,TCHAR* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}
void GB2312ToUTF_8(std::string& pOut,char *pText, int pLen)
{
	pOut.clear();

	int i = 0;

	while(i < pLen)
	{
		//如果是英文直接复制就可以
		if( pText[i] >= 0)
		{
			char asciistr[2]={0};
			asciistr[0] = (pText[i++]);
			pOut.append(asciistr);
		}
		else
		{
			char buf[4] = {0};
			
			WCHAR pbuffer;
			//TCHAR pbuffer;

			Gb2312ToUnicode(&pbuffer,pText+i);

			UnicodeToUTF_8(buf,&pbuffer);

			pOut.append(buf);

			i += 2;
		}
	}
	return;
}


std::string UrlUTF8(std::string tt)
{
	std::string dd;
	
	size_t len=tt.length();

	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)tt.at(i)))
		{
			char tempbuff[2]={0};
			sprintf(tempbuff,"%c",(BYTE)tt.at(i));
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)tt.at(i)))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
			dd.append(tempbuff);
		}
	}
	return dd;
}
std::string CreatInitXML(std::string strAPPName)
{
	std::string strMarks="\"";
	
	char msg[256]={0};
	
	std::string strRet;
	
	sprintf(msg,"<?xml version=%s1.0%s encoding=%sutf-8%s?><alarm><head><appname>%s</appname></head></alarm>",strMarks.c_str(),strMarks.c_str(),strMarks.c_str(),strMarks.c_str(),strAPPName.c_str());	
	
	GB2312ToUTF_8(strRet,msg,strlen(msg));
	
	return UrlUTF8(strRet);
}

std::string CreatWarXML(std::string strAPPName,int nAlarmLevel ,std::string strInfo)
{
	std::string strMarks="\"";
	char msg[256]={0};
	
	sprintf(msg,"<?xml version=%s1.0%s encoding=%sutf-8%s?><alarm><head><appname>%s</appname></head><body><level>%d</level><info>%s</info></body></alarm>"
		,strMarks.c_str(),strMarks.c_str(),strMarks.c_str(),strMarks.c_str(),strAPPName.c_str(),nAlarmLevel,strInfo.c_str());
	
	std::string strRet;

	GB2312ToUTF_8(strRet,msg,strlen(msg));

	return UrlUTF8(strRet);
}






















//void MC_InitLog(std::string strLogPath,comLib::Logger::LOG_LEVEL level)
//{
//	std::string strPath = strLogPath;
//	if(strPath.length() && strPath.at(strPath.length()-1)!='/')
//		strPath.append("/");
//	comLib::LogManager::CreateLongDirectory(strPath);
//	comLib::LogManager::SetLogFilePath(strLogPath.c_str());
//	comLib::LogManager::Open("MonitorClient");
//	comLib::LogManager::SetLogLevel(level);
//}
//
//bool MC_GetLogCfg()
//{
//	const std::string strIniFileName("./Config/config.ini");
//	if(!m_iniFile.LoadIniFile(strIniFileName.c_str()))
//	{
//		return false;
//	}
//	else
//	{
//		m_strLogPath = m_iniFile.GetString("SYSTEM", "log_path", "./log/");
//		if(m_strLogPath.empty())
//		{
//			m_strLogPath = "./log/";
//		}
//		if(m_strLogPath.at(m_strLogPath.length()-1) != '/')
//			m_strLogPath.append("/");
//
//		string strTemp;
//		strTemp = m_iniFile.GetStringLowercase("SYSTEM", "log_level", "warn");
//		if(strTemp == "fatal")
//			m_eLogLevel = comLib::Logger::LL_FATAL;
//		else if(strTemp == "error")
//			m_eLogLevel = comLib::Logger::LL_ERROR;
//		else if(strTemp == "warn")
//			m_eLogLevel = comLib::Logger::LL_WARN;
//		else if(strTemp == "trace")
//			m_eLogLevel = comLib::Logger::LL_TRACE;
//		else if(strTemp == "info")
//			m_eLogLevel = comLib::Logger::LL_INFO;
//		else if(strTemp == "all")
//			m_eLogLevel = comLib::Logger::LL_ALL;
//		else
//			m_eLogLevel = comLib::Logger::LL_DEBUG;
//
//		MC_InitLog(m_strLogPath,m_eLogLevel);
//		
//		return true;
//	}
//}


