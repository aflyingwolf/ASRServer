#include "stdafx.h"
#include "HttpReq.h"
namespace HttpReq
{
	WinHttp* WinHttp::m_pInstance=NULL;

	
	string_convert::string_convert(void)
	{
	}

	string_convert::~string_convert(void)
	{
	}

	string string_convert::ws2s(const wstring& ws)
	{
		string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
		setlocale(LC_ALL, "chs"); 
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		char *_Dest = new char[_Dsize];
		memset(_Dest,0,_Dsize);
		wcstombs(_Dest,_Source,_Dsize);
		string result = _Dest;
		delete []_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	wstring string_convert::s2ws(const string& s)
	{
		setlocale(LC_ALL, "chs"); 
		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		int nret = mbstowcs(_Dest,_Source,_Dsize);
		wstring result = _Dest;
		delete []_Dest;
		setlocale(LC_ALL, "C");
	  
		return result;
	 
	}
 
	wstring string_convert::UTF2Uni(const char* src, std::wstring &t)
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
	 
	 
	 
	int string_convert::Uni2UTF( const wstring& strRes, char *utf8, int nMaxSize )
	{
		if (utf8 == NULL) {
			return -1;
		}
		int len = 0;
		int size_d = nMaxSize;
	 
	 
		for (wstring::const_iterator it = strRes.begin(); it != strRes.end(); ++it)
		{
			wchar_t wchar = *it;
			if (wchar < 0x80)
			{  //
				//length = 1;
				utf8[len++] = (char)wchar;
			}
			else if(wchar < 0x800)
			{
				//length = 2;
	             
				if (len + 1 >= size_d)
					return -1;
	             
				utf8[len++] = 0xc0 | ( wchar >> 6 );
				utf8[len++] = 0x80 | ( wchar & 0x3f );
			}
			else if(wchar < 0x10000 )
			{
				//length = 3;
				if (len + 2 >= size_d)
					return -1;
	             
				utf8[len++] = 0xe0 | ( wchar >> 12 );
				utf8[len++] = 0x80 | ( (wchar >> 6) & 0x3f );
				utf8[len++] = 0x80 | ( wchar & 0x3f );
			}
			else if( wchar < 0x200000 ) 
			{
				//length = 4;
				if (len + 3 >= size_d)
					return -1;
	             
				utf8[len++] = 0xf0 | ( (int)wchar >> 18 );
				utf8[len++] = 0x80 | ( (wchar >> 12) & 0x3f );
				utf8[len++] = 0x80 | ( (wchar >> 6) & 0x3f );
				utf8[len++] = 0x80 | ( wchar & 0x3f );
			}
	     
		}
	     
	 
		return len;
	}
	 
	string string_convert::s2utfs(const  string&  strSrc)
	{
		string  strRes;
		wstring  wstrUni = s2ws(strSrc);
	     
		char*  chUTF8 = new char[wstrUni.length() * 3];
		memset(chUTF8,0x00,wstrUni.length() * 3);
		Uni2UTF(wstrUni,chUTF8, wstrUni.length() * 3);
		strRes = chUTF8;   
		delete  []chUTF8;
		return strRes;
	}
	 
	 
	string  string_convert::utfs2s(const string& strutf)
	{
		wstring  wStrTmp;
		UTF2Uni( strutf.c_str(),wStrTmp);
		return ws2s(wStrTmp);
	}
	WinHttp::WinHttp()
	{

	}
	WinHttp::~WinHttp()
	{
		
	}
	WinHttp* WinHttp::GetInstance()
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new WinHttp();
		}
		return m_pInstance;
	}
	HINTERNET WinHttp::OpenSession(LPCWSTR userAgent)
	{
	#ifdef USE_WINHTTP
		return WinHttpOpen(userAgent, NULL, NULL, NULL, NULL);;
	#else
		USES_CONVERSION; 
		return InternetOpen(W2CT(userAgent), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	#endif
	}

	HINTERNET WinHttp::Connect(HINTERNET hSession, LPCWSTR serverAddr, int portNo)
	{
	#ifdef USE_WINHTTP
		return WinHttpConnect(hSession, serverAddr, (INTERNET_PORT) portNo, 0);
	#else
		USES_CONVERSION; 
		return InternetConnect(hSession, W2CT(serverAddr), portNo, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	#endif
	}

	HINTERNET WinHttp::OpenRequest(HINTERNET hConnect, LPCWSTR verb, LPCWSTR objectName, int scheme)
	{
		DWORD flags = 0;
	#ifdef USE_WINHTTP
		if (scheme == INTERNET_SCHEME_HTTPS) {
			flags |= WINHTTP_FLAG_SECURE;
		}

		return WinHttpOpenRequest(hConnect, verb, objectName, NULL, NULL, NULL, flags);

	#else
		if (scheme == INTERNET_SCHEME_HTTPS) {
			flags |= INTERNET_FLAG_SECURE;
		}
		USES_CONVERSION; 
		return HttpOpenRequest(hConnect, W2CT(verb), W2CT(objectName), NULL, NULL, NULL, flags, 0);
	#endif
	}

	BOOL WinHttp::AddRequestHeaders(HINTERNET hRequest, LPCWSTR header)
	{
		SIZE_T len = lstrlenW(header);
	#ifdef USE_WINHTTP
		return WinHttpAddRequestHeaders(hRequest, header, DWORD(len), WINHTTP_ADDREQ_FLAG_ADD);
	#else
		USES_CONVERSION; 
		return HttpAddRequestHeaders(hRequest,W2CT(header), DWORD(len), HTTP_ADDREQ_FLAG_ADD);
	#endif
	}

	BOOL WinHttp::SendRequest(HINTERNET hRequest, const void* body, DWORD size)
	{
	#ifdef USE_WINHTTP
		return WinHttpSendRequest(hRequest, 0, 0, const_cast<void*>(body), size, size, 0);
	#else
		return HttpSendRequest(hRequest, 0, 0, const_cast<void*>(body), size);
	#endif
	}

	BOOL WinHttp::EndRequest(HINTERNET hRequest)
	{
	#ifdef USE_WINHTTP
		return WinHttpReceiveResponse(hRequest, 0);
	#else
		// if you use HttpSendRequestEx to send request then use HttpEndRequest in here!
		return TRUE;
	#endif
	}

	BOOL WinHttp::QueryInfo(HINTERNET hRequest, int queryId, char* szBuf, DWORD* pdwSize)
	{
	#ifdef USE_WINHTTP
		return WinHttpQueryHeaders(hRequest, (DWORD) queryId, 0, szBuf, pdwSize, 0);
	#else
		return HttpQueryInfo(hRequest, queryId, szBuf, pdwSize, 0);
	#endif
	}

	BOOL WinHttp::ReadData(HINTERNET hRequest, void* buffer, DWORD length, DWORD* cbRead)
	{
	#ifdef USE_WINHTTP
		return WinHttpReadData(hRequest, buffer, length, cbRead);
	#else
		return InternetReadFile(hRequest, buffer, length, cbRead);
	#endif
	}

	void WinHttp::CloseInternetHandle(HINTERNET hInternet)
	{
		if (hInternet)
		{
	#ifdef USE_WINHTTP
			WinHttpCloseHandle(hInternet);
	#else
			InternetCloseHandle(hInternet);
	#endif
		}
	}

	HttpRsp WinHttp::HttpRequest(char * szRequestUrl,const char* szPostData,int dataLen)
	{
		HINTERNET hSession = 0;
		HINTERNET hConnect = 0;
		HINTERNET hRequest = 0;
		HttpRsp httpRsp;
		CStringW Url=CA2W(szRequestUrl);
		CStringW strHeader(L"Content-type: application/x-www-form-urlencoded\r\n");
		//CStringW strHeader("Content-type: application/x-www-form-urlencoded\r\n");

		// Test data
		CrackedUrl crackedUrl(Url);
		
		// Open session.
		hSession = WinHttp::GetInstance()->OpenSession(L"HttpPost by CM_HomeBoy");
		//hSession = WinHttp::GetInstance()->OpenSession("HttpPost by CM_HomeBoy");
		if (hSession == NULL) {
			//printf("Error:Open session!\n");
			return HttpRsp::createErrRsp("Error:Open session!");
		}

		// Connect.
		hConnect = WinHttp::GetInstance()->Connect(hSession, crackedUrl.GetHostName(), crackedUrl.GetPort());
		if (hConnect == NULL) {
			//printf("Error:Connect failed!\n");
			return HttpRsp::createErrRsp("Error:Connect failed");
		}

		// Open request.
		hRequest = WinHttp::GetInstance()->OpenRequest(hConnect, L"POST", crackedUrl.GetPath(), crackedUrl.GetScheme());
		//hRequest = WinHttp::GetInstance()->OpenRequest(hConnect, "POST", crackedUrl.GetPath(), crackedUrl.GetScheme());
		if (hRequest == NULL) {
			//printf("Error:OpenRequest failed!\n");
			return HttpRsp::createErrRsp("Error:OpenRequest failed!");
		}

		// Add request header.
		if (!WinHttp::GetInstance()->AddRequestHeaders(hRequest, strHeader)) {
			//printf("Error:AddRequestHeaders failed!\n");
			return HttpRsp::createErrRsp("Error:AddRequestHeaders failed!");
		}

		// Send post data.
		if (!WinHttp::GetInstance()->SendRequest(hRequest, szPostData, dataLen)) {
			//printf("Error:SendRequest failed!\n");
			return HttpRsp::createErrRsp("Error:SendRequest failed!");
		}

		// End request
		if (!WinHttp::GetInstance()->EndRequest(hRequest)) {
			//printf("Error:EndRequest failed!\n");
			return HttpRsp::createErrRsp("Error:EndRequest failed!");
		}

		char szStatusBuf[HTTPREQ_STATUS_BUF_SIZE];
		wstring wsTemp;
		DWORD dwSize = 0;
		szStatusBuf[0] = 0;

		// Query header info.
	#ifdef USE_WINHTTP
		int contextLengthId = WINHTTP_QUERY_CONTENT_LENGTH;
		int statusCodeId = WINHTTP_QUERY_STATUS_CODE;
		int statusTextId = WINHTTP_QUERY_STATUS_TEXT;
	#else
		int contextLengthId = HTTP_QUERY_CONTENT_LENGTH;
		int statusCodeId = HTTP_QUERY_STATUS_CODE;
		int statusTextId = HTTP_QUERY_STATUS_TEXT;
	#endif
		dwSize = HTTPREQ_STATUS_BUF_SIZE;
		if (WinHttp::GetInstance()->QueryInfo(hRequest, contextLengthId, szStatusBuf, &dwSize)) {
			szStatusBuf[dwSize] = 0;
			//printf("Content length:[%s]\n", szBuf);
		}

		dwSize = HTTPREQ_STATUS_BUF_SIZE;
		if (WinHttp::GetInstance()->QueryInfo(hRequest, statusCodeId, szStatusBuf, &dwSize)) {
			szStatusBuf[dwSize] = 0;
			//printf("Status code:[%s]\n", szBuf);
			wsTemp=(wchar_t *)szStatusBuf;
			httpRsp.statusCode=string_convert::ws2s(wsTemp);
		}

		dwSize = HTTPREQ_STATUS_BUF_SIZE;
		if (WinHttp::GetInstance()->QueryInfo(hRequest, statusTextId, szStatusBuf, &dwSize)) {
			szStatusBuf[dwSize] = 0;
			//printf("Status text:[%s]\n", szBuf);
			wsTemp=(wchar_t *)szStatusBuf;
			httpRsp.statusText=string_convert::ws2s(wsTemp);
		}

		char szBodyBuf[HTTPREQ_BODY_BUF_SIZE];
		dwSize=HTTPREQ_BODY_BUF_SIZE;
		szBodyBuf[0] = 0;

		// read data.
		for (;;) {
			dwSize = HTTPREQ_BODY_BUF_SIZE;
			if (WinHttp::GetInstance()->ReadData(hRequest, szBodyBuf, dwSize, &dwSize) == FALSE) {
				break;
			}

			if (dwSize <= 0) {
				break;
			}

			szBodyBuf[dwSize] = 0;
			//printf("%s\n", szBuf);    //Output value = value1 + value2
			TrimHttpBody(szBodyBuf,dwSize);
			wsTemp=(wchar_t *)szBodyBuf;
			httpRsp.strBody.append(string_convert::ws2s(wsTemp));
		}

		WinHttp::GetInstance()->CloseInternetHandle(hRequest);
		WinHttp::GetInstance()->CloseInternetHandle(hConnect);
		WinHttp::GetInstance()->CloseInternetHandle(hSession);

		return httpRsp;
	}
	char * WinHttp::TrimHttpBody(char * pBody,int nLen)
	{
		char *p=pBody;
		for(int i=0;i<nLen-1;i++)
		{
			if ((*p=='\r')&&(*(p+1)=='\n'))
			{
				*p=0;
				*(p+1)=0;
				break;
			}
			p++;
		}
		return pBody;
	}

}