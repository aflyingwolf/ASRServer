#include "stdafx.h"
#include "ClientList.h"


ClientList::ClientList(void)
{
}


ClientList::~ClientList(void)
{
}
void ClientList::AddClient(Client *  pclient)
{
	this->_mutex.Lock();
	try
	{
		this->clientMap[pclient->pclientSocket->m_Socket]=pclient;
	}
	catch(...)
	{
		TRACE("��ӿͻ���ʧ��");
	}
	this->_mutex.Unlock();
}
void ClientList::Clear()
{
	this->_mutex.Lock();
	try
	{
		for(CLIENT_MAP_IT it=clientMap.begin();it!=clientMap.end();it++)
		{
			Client * pClient=(*it).second;
			//SOCKET�ر�
			if(pClient->pclientSocket!=NULL)
				closesocket(pClient->pclientSocket->m_Socket);

			delete pClient->pclientSocket;
			pClient->pclientSocket=NULL;
			delete pClient;
			pClient=NULL;
		}
		clientMap.clear();
	}
	catch(...)
	{
		TRACE("��տͻ���ʧ��");
	}
	this->_mutex.Unlock();
}
//���ҿͻ���
Client * ClientList::FindClient(SOCKET client)
{
	Client * pClient=NULL;
	this->_mutex.Lock();
	try
	{
		CLIENT_MAP_IT it=this->clientMap.find(client);
		if(it!=this->clientMap.end())
			pClient=it->second;
	}
	catch(...)
	{
		TRACE("FindClientʧ��");
	}
	this->_mutex.Unlock();
	return pClient;
}
void ClientList::ClientSend(SOCKET client,const char *szBuffer,int nSize)
{
	Client * pClient=NULL;
	this->_mutex.Lock();
	try
	{
		CLIENT_MAP_IT it=this->clientMap.find(client);
		if(it!=this->clientMap.end())
		{
			pClient=it->second;
			if(pClient!=NULL)
			{
				if(pClient->pclientSocket!=NULL)
				{
					pClient->pclientSocket->Send(szBuffer,nSize);
				}
			}
		}
	}
	catch(...)
	{
		TRACE("FindClientʧ��");
	}
	this->_mutex.Unlock();
}
//�Ƴ��ͻ���
Client * ClientList::RemoveClient(SOCKET client)
{
	Client * pClient=NULL;
	this->_mutex.Lock();
	try
	{
		CLIENT_MAP_IT it=this->clientMap.find(client);
		if(it!=this->clientMap.end())
		{
			pClient=it->second;
			clientMap.erase(it);
		}
	}
	catch(...)
	{
		TRACE("RemoveClientʧ��");
	}
	this->_mutex.Unlock();
	return pClient;
}