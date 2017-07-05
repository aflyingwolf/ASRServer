// ClientSocket.cpp: implementation of the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientSocket.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientSocket::CClientSocket()
{

}

CClientSocket::CClientSocket(SOCKET hSocket, SOCKADDR_IN sockaddrout)
{
	IsConnected = FALSE;
	m_Socket = hSocket;
	memcpy(&m_SocketAddrIN, &sockaddrout, sizeof(SOCKADDR_IN));
}

CClientSocket::~CClientSocket()
{
	CloseSocket(m_Socket);
}
