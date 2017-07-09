#pragma once
#include <string>
#include <clocale>

using namespace std;

class string_convert
{
public:
	string_convert(void);
	~string_convert(void);
public:
	static string ws2s(const wstring& ws);
	static wstring s2ws(const string& s);
	static wstring UTF2Uni(const char* src, std::wstring &t);
	static int Uni2UTF( const wstring& strRes, char *utf8, int nMaxSize );
	static string s2utfs(const  string&  strSrc);
	static string  utfs2s(const string& strutf);
};

