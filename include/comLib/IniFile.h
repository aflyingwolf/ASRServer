// TTIniFile_Imp.h: interface for the TTIniFile_Imp class.
// version: 2008-5-25 1.0
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTINIFILE_IMP_H__756960FC_834A_45A8_B4E9_2FA2398A80FC__INCLUDED_)
#define AFX_TTINIFILE_IMP_H__756960FC_834A_45A8_B4E9_2FA2398A80FC__INCLUDED_


#ifdef _MSC_VER
#pragma warning(disable:4786)	//for VC6 only
#pragma warning(disable:4503)
#endif

#include <string>
#include <map>
using namespace std;

namespace comLib
{
class IniFile
{
public:
	IniFile();
	~IniFile();
	bool	LoadIniFile(const char* szFileName);
	int		GetInteger(const char* szSection, const char* szKey, int nDefault);
	string	GetString(const char* szSection, const char* szKey, const char* szDefault);
	string	GetStringLowercase(const char* szSection, const char* szKey, const char* szDefault);
	bool	GetBool(const char* szSection, const char* szKey, bool bDefault);

	int		GetIntegerEx(const char* szSection, const char* keyPrefix, int index, int nDefault);
	string	GetStringEx(const char* szSection, const char* keyPrefix, int index, const char* szDefault);
	string	GetStringLowercaseEx(const char* szSection, const char* keyPrefix, int index, const char* szDefault);
	bool	GetBoolEx(const char* szSection, const char* keyPrefix, int index, bool bDefault);

	void	SetInteger(const char* szSection, const char* szKey, int value){};
	void	SetString(const char* szSection, const char* szKey, const char* value){};
	void	SetBool(const char* szSection, const char* szKey, bool bDefault){};
	string  makeSec(const char* keyPrefix, int index) const;
private:
	typedef std::map<std::string, std::string> TSection;
	std::map<std::string, TSection*> m_Inifile;
	inline void clear();
	inline char getLowercase(char in);
	inline bool analyzeLine(std::string& strCurrSec, const char* szLine);
	inline void addASection(const std::string& strSection);
	inline void addAValue(const std::string& strSection, const std::string& strP, const std::string& strV);
	inline TSection* getSection(const char* szSection);
	inline const char* getValue(const TSection* pSec, const char* szP);
	
};
} //namespace comLib

#endif // !defined(AFX_TTINIFILE_IMP_H__756960FC_834A_45A8_B4E9_2FA2398A80FC__INCLUDED_)
