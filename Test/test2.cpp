// WinHttpTest.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include <stdafx.h>
#include <vector>
#include <winsock2.h>
#include <Winhttp.h>
//#include <urlmon.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "AtlBase.h"
#include "AtlConv.h"
using namespace std;
#pragma comment(lib, "winhttp")//��һ�䲻��ʡ��
string GetHost(string strUrl)
{
   int indexHttp = strUrl.find("http://");
   if(indexHttp != -1)
   {
       strUrl = strUrl.substr(7);
   }
   else
       return "";
   int indexSlash = strUrl.find("/");
   if(indexSlash != -1)
   {
       return strUrl.substr(0, indexSlash);
   }
   else
       return strUrl;
   return "";
}
string GetRequestStr(string strUrl)
{
   int indexHttp = strUrl.find("http://");
   if(indexHttp != -1)
   {
       strUrl = strUrl.substr(7);
   }
   else
       return "";
   int indexSlash = strUrl.find("/");
   if(indexSlash == -1)
   {
       return "";
   }
   else
       return strUrl.substr(indexSlash);
}
void GetHtml(string strUrl)
{
   string strHost = GetHost(strUrl);//��ȡHost
   string strRequestStr = GetRequestStr(strUrl);//��ȡ����·��
   USES_CONVERSION;
    //2014��7��9��10:02:29
    //LPCWSTR�Ķ��� typedef const wchar_t* LPCWSTR;
    //LPSTR�Ķ���   typedef char* LPCWSTR;
    //LPWSTR�Ķ���  typedef wchar_t* LPWSTR;
   LPCWSTR host = A2CW(strHost.c_str());//stringת��Ϊ����ָ������
   LPCWSTR requestStr = A2CW(strRequestStr.c_str());
   //Variables
   DWORD dwSize = 0;
   BOOL  bResults = FALSE;
   
   //Note the definition of HINTERNET
   HINTERNET  hSession = NULL,
       hConnect = NULL,
       hRequest = NULL;
       
   //2014��7��9��10:39:33
   //Search the WinHttp API
   //what to do when call the function WinHttpOpen��
   // Use WinHttpOpen to obtain a session handle.
   hSession = WinHttpOpen(L"WinHTTP Example/1.0",
       WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
       WINHTTP_NO_PROXY_NAME,
       WINHTTP_NO_PROXY_BYPASS, 0);
 // Specify an HTTP server.
   if (hSession)
       hConnect = WinHttpConnect(hSession, host,
       INTERNET_DEFAULT_HTTP_PORT, 0);
 // Create an HTTP request handle.
   if (hConnect)
       hRequest = WinHttpOpenRequest(hConnect, L"GET", requestStr,
       NULL, WINHTTP_NO_REFERER,
       NULL,
       NULL);
 // Send a request.
   if (hRequest)
       bResults = WinHttpSendRequest(hRequest,
       WINHTTP_NO_ADDITIONAL_HEADERS,
       0, WINHTTP_NO_REQUEST_DATA, 0,
       0, 0);
// End the request.
   if (bResults)
       bResults = WinHttpReceiveResponse(hRequest, NULL);
       
//2014��7��9��16:35:40  
//��ȡ����Header
     LPVOID lpOutBuffer = NULL;
     int temp;
     if (bResults)
    {
        //�˴��Ĳ������ȡ��Ӧ��Header������ͬ
        WinHttpQueryHeaders( hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF|WINHTTP_QUERY_FLAG_REQUEST_HEADERS,
                             WINHTTP_HEADER_NAME_BY_INDEX, NULL,
                             &dwSize, WINHTTP_NO_HEADER_INDEX);

        // Allocate memory for the buffer.
        if( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
        {
            temp = dwSize/sizeof(WCHAR);
            lpOutBuffer = new WCHAR[temp];

            // Now, use WinHttpQueryHeaders to retrieve the header.
            bResults = WinHttpQueryHeaders( hRequest,
                                       WINHTTP_QUERY_RAW_HEADERS_CRLF|WINHTTP_QUERY_FLAG_REQUEST_HEADERS,
                                       WINHTTP_HEADER_NAME_BY_INDEX,
                                       lpOutBuffer, &dwSize,
                                       WINHTTP_NO_HEADER_INDEX);
        }
        
        
    }
    //���������ʽ�������header���˴�cout�в�ͨ
    wcout<<(WCHAR*)lpOutBuffer;
    if (bResults)
       printf("\n%S",lpOutBuffer);
       
    delete [] lpOutBuffer;
 
 //2014��7��9��14:43:20 ��ȡ��Ӧͷ��
     if (bResults)
    {
        WinHttpQueryHeaders( hRequest, WINHTTP_QUERY_RAW_HEADERS_CRLF,
                             WINHTTP_HEADER_NAME_BY_INDEX, NULL,
                             &dwSize, WINHTTP_NO_HEADER_INDEX);

        // Allocate memory for the buffer.
        if( GetLastError() == ERROR_INSUFFICIENT_BUFFER )
        {
            temp = dwSize/sizeof(WCHAR);
            lpOutBuffer = new WCHAR[temp];

            // Now, use WinHttpQueryHeaders to retrieve the header.
            bResults = WinHttpQueryHeaders( hRequest,
                                       WINHTTP_QUERY_RAW_HEADERS_CRLF,
                                       WINHTTP_HEADER_NAME_BY_INDEX,
                                       lpOutBuffer, &dwSize,
                                       WINHTTP_NO_HEADER_INDEX);
        }
        
        
    }
    //���������ʽ�������header���˴�cout�в�ͨ
    wcout<<(WCHAR*)lpOutBuffer;
    if (bResults)
       printf("\n%S",lpOutBuffer);
       
    delete [] lpOutBuffer;
}
int _tmain3(int argc, _TCHAR* argv[])
{

   GetHtml("http://bbs.bccn.net/thread-294526-1-1.html");
   system("pause");
   return 0;
}