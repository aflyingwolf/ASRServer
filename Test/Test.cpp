// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

//将wchar_t* 转成char*的实现函数如下：

char *w2c1(char *pcstr,const wchar_t *pwstr, size_t len)

{

int nlength=wcslen(pwstr);

//获取转换后的长度

int nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion

0,         // no special flags to handle unmapped characters

pwstr,     // wide character string to convert

nlength,   // the number of wide characters in that string

NULL,      // no output buffer given, we just want to know how long it needs to be

0,

NULL,      // no replacement character given

NULL );    // we don't want to know if a character didn't make it through the translation

// make sure the buffer is big enough for this, making it larger if necessary

if(nbytes>len)   nbytes=len;

// 通过以上得到的结果，转换unicode 字符为ascii 字符

WideCharToMultiByte( 0, // specify the code page used to perform the conversion

0,         // no special flags to handle unmapped characters

pwstr,   // wide character string to convert

nlength,   // the number of wide characters in that string

pcstr, // put the output ascii characters at the end of the buffer

nbytes,                           // there is at least this much space there

NULL,      // no replacement character given

NULL );

return pcstr ;

}

void test0()
{
	
	char szBuf[512];
	memset(szBuf,0,512);
	szBuf[0]='2';
	szBuf[1]=0;
	szBuf[2]='0';
	szBuf[3]=0;
	szBuf[4]='0';
	szBuf[5]=0;
	wchar_t * pwstr =(wchar_t *)szBuf;
	
	char *pcstr = (char *)malloc(sizeof(char)*(2 * wcslen(pwstr)+1));

    memset(pcstr , 0 , 2 * wcslen(pwstr)+1 );

    w2c1(pcstr,pwstr,2 * wcslen(pwstr)+1) ;
	cout<<pcstr<<endl;
	return;
}
wstring UTF2Uni(const char* src, std::wstring &t)
{
    if (src == NULL) 
    {
        return L"";
    }
     
    int size_s = strlen(src);
    int size_d = size_s + 10;          //?
     
    wchar_t *des = new wchar_t[size_d];
    memset(des, 0, size_d * sizeof(wchar_t));
     
    int s = 0, d = 0;
    bool toomuchbyte = true; //set true to skip error prefix.
     
    while (s < size_s && d < size_d)
    {
        unsigned char c = src[s];
        if ((c & 0x80) == 0) 
        {
            des[d++] += src[s++];
        } 
        else if((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
        {
            wchar_t &wideChar = des[d++];
            wideChar  = (src[s + 0] & 0x3F) << 6;
            wideChar |= (src[s + 1] & 0x3F);
             
            s += 2;
        }
        else if((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
        {
            wchar_t &wideChar = des[d++];
             
            wideChar  = (src[s + 0] & 0x1F) << 12;
            wideChar |= (src[s + 1] & 0x3F) << 6;
            wideChar |= (src[s + 2] & 0x3F);
             
            s += 3;
        } 
        else if((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
        {
            wchar_t &wideChar = des[d++];
             
            wideChar  = (src[s + 0] & 0x0F) << 18;
            wideChar  = (src[s + 1] & 0x3F) << 12;
            wideChar |= (src[s + 2] & 0x3F) << 6;
            wideChar |= (src[s + 3] & 0x3F);
             
            s += 4;
        } 
        else 
        {
            wchar_t &wideChar = des[d++]; ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
             
            wideChar  = (src[s + 0] & 0x07) << 24;
            wideChar  = (src[s + 1] & 0x3F) << 18;
            wideChar  = (src[s + 2] & 0x3F) << 12;
            wideChar |= (src[s + 3] & 0x3F) << 6;
            wideChar |= (src[s + 4] & 0x3F);
             
            s += 5;
        }
    }
     
    t = des;
    delete[] des;
    des = NULL;
     
    return t;
}

void test1()
{
		setlocale(LC_ALL, "chs"); 
		size_t _Ssize = 512;
		char _Source [512];
		memset(_Source,0,_Ssize);
		_Source[0]='2';
		_Source[1]=0;
		_Source[2]='0';
		_Source[3]=0;
		_Source[4]='0';
		_Source[5]=0;
		size_t _Dsize = _Ssize + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		int nret = mbstowcs(_Dest,_Source,_Dsize);
		delete []_Dest;
		setlocale(LC_ALL, "C");
	
}
void test2()
{
	wchar_t  ws[1024];
	swprintf(ws, 100, L"%s", "200");
	cout<<ws<<endl;
}

void test3()
{
	size_t _Ssize = 512;
	char _Source [512];
	memset(_Source,0,_Ssize);
	_Source[0]='2';
	_Source[1]=0;
	_Source[2]='0';
	_Source[3]=0;
	_Source[4]='0';
	_Source[5]=0;
	wstring ws;	
	ws=UTF2Uni(_Source,ws);
	return;
}

int main(int argc, char* argv[])
{
	
	test0();
	char ch;
	cin>>ch;
	return 0;
}
