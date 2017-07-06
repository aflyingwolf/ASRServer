#include "MC_HttpReq.h"

WinHttp* WinHttp::m_pInstance=NULL;

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
BOOL WinHttp::HttpRequest(CStringW strRequestUrl,CStringW PostData)
{
	HINTERNET hSession = 0;
	HINTERNET hConnect = 0;
	HINTERNET hRequest = 0;
	CStringW strHeader(L"Content-type: application/x-www-form-urlencoded\r\n");
	//CStringW strHeader("Content-type: application/x-www-form-urlencoded\r\n");

	// Test data
	CrackedUrl crackedUrl(strRequestUrl);
	CStringA strPostData(PostData);

	// Open session.
	hSession = WinHttp::GetInstance()->OpenSession(L"HttpPost by CM_HomeBoy");
	//hSession = WinHttp::GetInstance()->OpenSession("HttpPost by CM_HomeBoy");
	if (hSession == NULL) {
		//printf("Error:Open session!\n");
		return -1;
	}

	// Connect.
	hConnect = WinHttp::GetInstance()->Connect(hSession, crackedUrl.GetHostName(), crackedUrl.GetPort());
	if (hConnect == NULL) {
		//printf("Error:Connect failed!\n");
		return -1;
	}

	// Open request.
	hRequest = WinHttp::GetInstance()->OpenRequest(hConnect, L"POST", crackedUrl.GetPath(), crackedUrl.GetScheme());
	//hRequest = WinHttp::GetInstance()->OpenRequest(hConnect, "POST", crackedUrl.GetPath(), crackedUrl.GetScheme());
	if (hRequest == NULL) {
		//printf("Error:OpenRequest failed!\n");
		return -1;
	}

	// Add request header.
	if (!WinHttp::GetInstance()->AddRequestHeaders(hRequest, strHeader)) {
		//printf("Error:AddRequestHeaders failed!\n");
		return -1;
	}

	// Send post data.
	if (!WinHttp::GetInstance()->SendRequest(hRequest, (const char*)strPostData, strPostData.GetLength())) {
		//printf("Error:SendRequest failed!\n");
		return -1;
	}

	// End request
	if (!WinHttp::GetInstance()->EndRequest(hRequest)) {
		//printf("Error:EndRequest failed!\n");
		return -1;
	}

	char szBuf[BUF_SIZE];
	DWORD dwSize = 0;
	szBuf[0] = 0;

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
	dwSize = BUF_SIZE;
	if (WinHttp::GetInstance()->QueryInfo(hRequest, contextLengthId, szBuf, &dwSize)) {
		szBuf[dwSize] = 0;
		//printf("Content length:[%s]\n", szBuf);
	}

	dwSize = BUF_SIZE;
	if (WinHttp::GetInstance()->QueryInfo(hRequest, statusCodeId, szBuf, &dwSize)) {
		szBuf[dwSize] = 0;
		//printf("Status code:[%s]\n", szBuf);
	}

	dwSize = BUF_SIZE;
	if (WinHttp::GetInstance()->QueryInfo(hRequest, statusTextId, szBuf, &dwSize)) {
		szBuf[dwSize] = 0;
		//printf("Status text:[%s]\n", szBuf);
	}

	// read data.
	for (;;) {
		dwSize = BUF_SIZE;
		if (WinHttp::GetInstance()->ReadData(hRequest, szBuf, dwSize, &dwSize) == FALSE) {
			break;
		}

		if (dwSize <= 0) {
			break;
		}

		szBuf[dwSize] = 0;
		//printf("%s\n", szBuf);    //Output value = value1 + value2
	}

	WinHttp::GetInstance()->CloseInternetHandle(hRequest);
	WinHttp::GetInstance()->CloseInternetHandle(hConnect);
	WinHttp::GetInstance()->CloseInternetHandle(hSession);
	return 0;
}