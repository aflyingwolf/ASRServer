// BaseSocket.h: interface for the CBaseSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASESOCKET_H__BDF824C2_B6CE_4E24_AF54_AE61CB2C0C9B__INCLUDED_)
#define AFX_BASESOCKET_H__BDF824C2_B6CE_4E24_AF54_AE61CB2C0C9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseSocket  
{
public:
	SOCKET Socket();
	SOCKADDR_IN m_SocketAddrIN;

	BOOL GetMachineIP(CString & strIP);
	BOOL CloseSocket(SOCKET s);
	BOOL AsyncSelect(HWND hWnd, unsigned int wMsg, long lEvent);
	BOOL Accept( unsigned int &nSocket, CString &sRAddr);
	virtual int Accept(SOCKADDR_IN *sockaddr);
	BOOL Bind(const unsigned short usLPort);
	BOOL Listen(const int iListenNum);
	BOOL Send(const char * szbuffer,int nLen);
	int Recv(char * buffer);
	BOOL ConnectSocket( const unsigned short usHostPort,  const char *sHostAddr);
	BOOL InitSocket();
	SOCKET m_Socket;
	CBaseSocket();
	virtual ~CBaseSocket();

};

#endif // !defined(AFX_BASESOCKET_H__BDF824C2_B6CE_4E24_AF54_AE61CB2C0C9B__INCLUDED_)
