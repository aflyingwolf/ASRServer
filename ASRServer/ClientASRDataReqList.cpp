#include "stdafx.h"
#include "ClientASRDataReqList.h"


ClientASRDataReqList::ClientASRDataReqList(void)
{
}


ClientASRDataReqList::~ClientASRDataReqList(void)
{
}
bool ClientASRDataReqList::AddList(ClientASRDataReq req)
{
	_mutex.Lock();
	bool ret=false;
	try
	{
		this->reqList.push_back(req);
		ret=true;
	}
	catch(...)
	{
		exception ex("ClientASRDataReqList::AddList error");
		throw &ex;
	}
	_mutex.Unlock();
	return ret;
}
bool ClientASRDataReqList::isEmpty()
{
	_mutex.Lock();
	bool ret=true;
	try
	{
		ret=this->reqList.empty();
	}
	catch(...)
	{
		exception ex("ClientASRDataReqList::isEmpty error");
		throw &ex;
	}
	_mutex.Unlock();
	return ret;

}
bool ClientASRDataReqList::GetASRReq(ClientASRDataReq & retReq)
{
	_mutex.Lock();
	bool ret=false;
	try
	{
		if(!this->reqList.empty())
		{
			retReq=this->reqList.front();
			this->reqList.pop_front();
			ret=true;
		}
	}
	catch(...)
	{
		return false;
	}
	_mutex.Unlock();
	return ret;
}