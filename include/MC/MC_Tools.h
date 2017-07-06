#pragma once
#include "comLib/IniFile.h"
#include "comLib/Logger.h"
#include <windows.h>
#include <iostream>
#include <time.h> 

std::string UrlUTF8(char * str);

std::string string_To_UTF8(const std::string & str);

std::string CreatInitXML(std::string strAPPName);

std::string CreatWarXML(std::string strAPPName,int nAlarmLevel ,std::string strInfo);

//comLib::IniFile m_iniFile;	
//
//std::string m_strLogPath;
//
//comLib::Logger::LOG_LEVEL m_eLogLevel;
//
//bool MC_GetLogCfg();







