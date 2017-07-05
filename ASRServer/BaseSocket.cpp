// BaseSocket.cpp: implementation of the CBaseSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseSocket::CBaseSocket()
{
	WSADATA WSAData;
	int nRc = WSAStartup(0x0101, &WSAData);
	if (nRc || WSAData.wVersion != 0x0101) {
		 WSACleanup();
		 throw ;
	}
}

CBaseSocket::~CBaseSocket()
{
	if(m_Socket != NULL)
		CloseSocket(m_Socket);
	WSACleanup();
}

BOOL CBaseSocket::InitSocket()
{
	m_Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_Socket == INVALID_SOCKET)
	{
		AfxMessageBox("¾¯¸æ£¡Socket½¨Á¢Ê§°Ü£¡",MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	return TRUE;
}

BOOL CBaseSocket::ConnectSocket(const unsigned short usHostPort, const char *sHostAddr)
{
   //SOCKADDR_IN SockAddrIn;

   m_SocketAddrIN.sin_family = AF_INET;
   m_SocketAddrIN.sin_port = htons( usHostPort );
   m_SocketAddrIN.sin_addr.s_addr = inet_addr( sHostAddr );
   _strnset( m_SocketAddrIN.sin_zero, NULL, 8 );

   return ( connect( m_Socket, (LPSOCKADDR)&m_SocketAddrIN, sizeof( sockaddr ) ) != SOCKET_ERROR );
}

int CBaseSocket::Recv(char * buffer)
{
	int  nRecvBytes;
	nRecvBytes = recv(m_Socket, buffer, BLOCK_SIZE, 0);
	if(nRecvBytes == SOCKET_ERROR)
		return -1;

	return nRecvBytes;
}

BOOL CBaseSocket::Send(const char * szbuffer,int nSize)
{
	int nSendResu;
	nSendResu = send(m_Socket,szbuffer, nSize, 0);
	if(nSendResu == SOCKET_ERROR)
	{
		int nSockErr = WSAGetLastError(); 
		return FALSE;
	}
	return TRUE;
}

BOOL CBaseSocket::Listen(const int iListenNum)
{
   if ( m_Socket == INVALID_SOCKET ) 
	   return FALSE;
   return ( listen( m_Socket, iListenNum ) != SOCKET_ERROR );
}

BOOL CBaseSocket::Bind(const unsigned short usLPort)
{
   if ( m_Socket == INVALID_SOCKET ) return false;

   SOCKADDR_IN SockAddrIn;

   SockAddrIn.sin_family = AF_INET;
   SockAddrIn.sin_port = htons( usLPort );
   SockAddrIn.sin_addr.s_addr = htonl( INADDR_ANY );
   _strnset( SockAddrIn.sin_zero, NULL, 8 );
   int ret=bind( m_Socket, (LPSOCKADDR)&SockAddrIn, sizeof( sockaddr ));
   if(ret==SOCKET_ERROR)
   {
	   int nSockErr = WSAGetLastError(); 
	   return false;
   }
   else return true;
}

int CBaseSocket::Accept(SOCKADDR_IN *sockaddr)
{
	SOCKET nConnSock;
	int nAddrLen = sizeof(SOCKADDR_IN);
	nConnSock = accept(m_Socket, (LPSOCKADDR)sockaddr, &nAddrLen);
	if(nConnSock == INVALID_SOCKET )
	{
		int nSockErr = WSAGetLastError(); 
		return -1;
	}
	return nConnSock;
}

BOOL CBaseSocket::Accept(unsigned int &nSocket, CString &sRAddr)
{
	if ( m_Socket == INVALID_SOCKET ) return false;

	sockaddr_in SockAddr;
	int iLen = sizeof( sockaddr_in );

	nSocket = accept( m_Socket, (sockaddr *)&SockAddr, &iLen );
	if ( nSocket == INVALID_SOCKET )
		return false;
	else
	{
		sRAddr = inet_ntoa( SockAddr.sin_addr );
		return true;
	}
}

BOOL CBaseSocket::AsyncSelect(HWND hWnd, unsigned int wMsg, long lEvent)
{
	int nSeleErr = WSAAsyncSelect(m_Socket, hWnd, wMsg, lEvent);
	if( nSeleErr == SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;  // 0 : success!!!
}

BOOL CBaseSocket::CloseSocket(SOCKET s)
{
	return closesocket(s);
}

BOOL CBaseSocket::GetMachineIP(CString &strIP)
{
	char RealIP[20];
	gethostname(RealIP,20);
	HOSTENT *hostent;
	hostent = gethostbyname(RealIP);
	if ( hostent == NULL)
		return FALSE;
	strIP = inet_ntoa(*(LPIN_ADDR)*(hostent->h_addr_list));	
	return TRUE;
}

SOCKET CBaseSocket::Socket()
{
	SOCKET nSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(nSocket == INVALID_SOCKET)
	{
		int nSockErr = WSAGetLastError(); 
		//write to view
		return -nSockErr;
	}
	m_Socket = nSocket;
	return m_Socket; //returns a descriptor referencing the new socket
}
