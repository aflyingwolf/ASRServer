#pragma once
#include <map>
#include <string>
#include "Client.h"
#include "MyMutex.h"
using namespace std;
typedef map<SOCKET,Client *> CLIENT_MAP;
typedef CLIENT_MAP::iterator CLIENT_MAP_IT;
class ClientList
{
public:
	ClientList(void);
	~ClientList(void);
public:
	//客户端列表
	CLIENT_MAP clientMap;
	MyMutex _mutex;
public:
	//添加客户端
	void AddClient(Client * pclient);
	//查找客户端
	Client * FindClient(SOCKET client);
	//移除客户端，注意，对象资源由你自己释放
	Client * RemoveClient(SOCKET client);
	//删除客户端列表,注意对象资源会自动释放
	void Clear();
	//某客户端数据发送
	void ClientSend(SOCKET client,const char *szBuffer,int nSize);
};

