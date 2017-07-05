#pragma once
#include <string>
#include "ClientSocket.h"
using namespace std;
class Client
{
public:
	Client(string ip,int port,CClientSocket * pclientSocket);
	~Client(void);
public:
	string ip;
	int port;
	//客户端SOCKET指针，记得要手动删除
	CClientSocket * pclientSocket;
};

