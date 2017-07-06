// TTIniFile_Imp.cpp: implementation of the TTIniFile_Imp class.
// version: 2009-11-6 1.1
//////////////////////////////////////////////////////////////////////
#include "comLib/IniFile.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace comLib
{

IniFile::IniFile()
{

}

IniFile::~IniFile()
{
	clear();
}

int IniFile::GetInteger(const char* szSection, const char* szKey, int nDefault)
{
	TSection* p = getSection(szSection);
	if(!p)
		return nDefault;
	const char* szValue = getValue(p, szKey);
	if(!szValue)
		return nDefault;
    else if ('0' == szValue[0]
        &&('x'==szValue[1]||'X'==szValue[1]))
    {
        int nRet=0;
        sscanf(szValue,"%x",&nRet);
        return nRet;
    }
	return atoi(szValue);
}

std::string IniFile::GetString(const char* szSection, const char* szKey, const char* szDefault)
{
	TSection* p = getSection(szSection);
	if(!p)
		return szDefault;
	const char* szValue = getValue(p, szKey);
	if(!szValue)
		return szDefault;
	return szValue;
}

std::string IniFile::GetStringLowercase(const char* szSection, const char* szKey, const char* szDefault)
{
	TSection* p = getSection(szSection);
	if(!p)
		return szDefault;
	const char* szValue = getValue(p, szKey);
	if(!szValue)
		return szDefault;
	std::string str;
	while(*szValue)
	{
		str += getLowercase(*szValue);
		++szValue;
	}
	return str;
}

bool IniFile::GetBool(const char* szSection, const char* szKey, bool bDefault)
{
	std::string str = GetStringLowercase(szSection, szKey, bDefault?"yes":"no");
	return "yes"==str || "true"==str;
}

int	IniFile::GetIntegerEx(const char* szSection, const char* keyPrefix, int index, int nDefault)
{
	return GetInteger(szSection, makeSec(keyPrefix, index).c_str(), nDefault);
}

std::string IniFile::GetStringEx(const char* szSection, const char* keyPrefix, int index, const char* szDefault)
{
	return GetString(szSection, makeSec(keyPrefix, index).c_str(), szDefault);
}

std::string IniFile::GetStringLowercaseEx(const char* szSection, const char* keyPrefix, int index, const char* szDefault)
{
	return GetStringLowercase(szSection, makeSec(keyPrefix, index).c_str(), szDefault);
}

bool IniFile::GetBoolEx(const char* szSection, const char* keyPrefix, int index, bool bDefault)
{
	return GetBool(szSection, makeSec(keyPrefix, index).c_str(), bDefault);
}

string IniFile::makeSec(const char* keyPrefix, int index) const
{
	std::ostringstream s;
	s<<keyPrefix<<index;
	return s.str();
}

inline void IniFile::clear()
{
	std::map<std::string, TSection*>::iterator iter = m_Inifile.begin();
	for(; iter != m_Inifile.end(); ++iter)
	{
		TSection* p = iter->second;
		delete p;
	}
	m_Inifile.clear();
}

inline IniFile::TSection* IniFile::getSection(const char* szSection)
{
	std::string strSec;
	while(*szSection)
	{
		strSec += getLowercase(*szSection);
		szSection++;
	}
	
	std::map<std::string, TSection*>::iterator iter = m_Inifile.find(strSec);
	if(iter == m_Inifile.end())
		return 0;
	return iter->second;
}

inline const char* IniFile::getValue(const TSection* pSec, const char* szP)
{
	std::string strP;
	while(*szP)
	{
		strP += getLowercase(*szP);
		szP++;
	}

	TSection::const_iterator iter = pSec->find(strP);
	if(iter == pSec->end())
		return 0;
	const std::string& str = iter->second;
	return str.c_str();
}

inline char IniFile::getLowercase(char in)
{
	if(in>='A' && in<='Z')
		return in - 'A' + 'a';
	return in;
}

inline void IniFile::addASection(const std::string& strSection)
{
	//only add the first one
	if(m_Inifile.find(strSection) != m_Inifile.end())
		return;

	m_Inifile[strSection] = new TSection;
}

inline void IniFile::addAValue(const std::string& strSection, const std::string& strP, const std::string& strV)
{
	std::map<std::string, TSection*>::iterator iter =  m_Inifile.find(strSection);
	if(iter == m_Inifile.end())
		return;
	TSection* pSec = iter->second;

	//only do add action on the 1st time
	if(pSec->find(strP) != pSec->end())
		return;
	(*pSec)[strP] = strV;
//	std::cout<<strSection << " "<< strP<< "="<<strV<<"<"<<std::endl;
}

bool IniFile::LoadIniFile(const char* szFileName)
{
	clear();
	std::ifstream inif(szFileName);
//	if(!inif.good())	//AIX5.3下不能使用该判断，也不能使用if(!inif)——可能是C++运行库版本问题
	if(!inif.is_open())
		return false;
	const int CFG_LINE_LEN = 1024;
	char line[CFG_LINE_LEN];
	std::string strCurrSection;
	while(!inif.eof())
	{
		inif.getline(line, CFG_LINE_LEN);
		if(strlen(line) == 0)
			continue;
		if(!analyzeLine(strCurrSection, line))
			break;
	}
	return true;
}

//special marks in a inifile: 
//[ ] # ; ' " = space
inline bool IniFile::analyzeLine(std::string& strCurrSec, const char* szLine)
{
	char* pEqMark = strchr((char*)szLine, '=');
	if(!pEqMark)	//may be a section name
	{
		std::string strSec;
		bool bSection = false;
		for(int i=0; ;++i)
		{
			switch(szLine[i])
			{
			case ' ':
				break;				//spaces in section name will be skipped;
			case '[':
				if(bSection)		//like: "[x["
					return true;
				bSection = true;
				break;
			case ']':
				if(strSec.length())
				{
					strCurrSec = strSec;	//set a new current section
					addASection(strSec);
				}
				return true;		//like: "]"
			//finish marks.
			case '#':
			case ';':
			case '\r':				//on linux \r is the last character in a line
			case 0:
				return true;
			default:
				if(bSection)
					strSec += getLowercase(szLine[i]);
				else				//like: "a["
					return true;
				break;
			}
		}
	}
	else
	{
		if(!strCurrSec.length())
			return true;

		const char* p = szLine;
		//get parameter 1st
		std::string strPara;
		while(p != pEqMark)
		{
			switch(*p)
			{
			case ' ':
				break;
			case '#':
			case ';':
			case '\r':
			case 0:
				return true;
			//invalid characters.
			case '[':
			case ']':
			case '\"':
			case '\'':
				return true;
			default:
				strPara += getLowercase(*p);
			}
			++p;
		}
		if(!strPara.length())
			return true;

		++p;	//skip the '=' mark
		std::string strValue;
		bool bInBarcket = false;	//is there a ' or a "?
		for(;;)
		{
			switch(*p)
			{
			case ' ':
				if(bInBarcket)
					strValue += *p;
				break;
			case '\'':
			case '\"':
				if(bInBarcket)		//finished
				{
					addAValue(strCurrSec, strPara, strValue);
					return true;
				}
				else
					bInBarcket = true;
				break;
			case '#':
			case ';':
			case '\r':
			case 0:
				addAValue(strCurrSec, strPara, strValue);
				return true;
			default:
				strValue += *p;
			}
			++p;
		}
		return true;
	}
}

} //namespace comLib
