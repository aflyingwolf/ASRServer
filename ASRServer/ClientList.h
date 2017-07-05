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
	//�ͻ����б�
	CLIENT_MAP clientMap;
	MyMutex _mutex;
public:
	//��ӿͻ���
	void AddClient(Client * pclient);
	//���ҿͻ���
	Client * FindClient(SOCKET client);
	//�Ƴ��ͻ��ˣ�ע�⣬������Դ�����Լ��ͷ�
	Client * RemoveClient(SOCKET client);
	//ɾ���ͻ����б�,ע�������Դ���Զ��ͷ�
	void Clear();
	//ĳ�ͻ������ݷ���
	void ClientSend(SOCKET client,const char *szBuffer,int nSize);
};

