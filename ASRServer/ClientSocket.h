// ClientSocket.h: interface for the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKET_H__D3DFCFCA_F3BC_4B6A_94A8_889E43CC0D6D__INCLUDED_)
#define AFX_CLIENTSOCKET_H__D3DFCFCA_F3BC_4B6A_94A8_889E43CC0D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseSocket.h"

class CClientSocket : public CBaseSocket  
{
public:
	BOOL IsConnected;
	CClientSocket(SOCKET hSocket, SOCKADDR_IN sockaddrout);
	CClientSocket();
	virtual ~CClientSocket();

};

#endif // !defined(AFX_CLIENTSOCKET_H__D3DFCFCA_F3BC_4B6A_94A8_889E43CC0D6D__INCLUDED_)
