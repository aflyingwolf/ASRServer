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
	//�ͻ���SOCKETָ�룬�ǵ�Ҫ�ֶ�ɾ��
	CClientSocket * pclientSocket;
};

