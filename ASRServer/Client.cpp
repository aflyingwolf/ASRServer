#include "stdafx.h"
#include "Client.h"


Client::Client(string ip,int port,CClientSocket *pclientSocket)
	:ip(ip),port(port),pclientSocket(pclientSocket)
{
}


Client::~Client(void)
{
	
}
