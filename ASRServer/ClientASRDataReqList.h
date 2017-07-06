#pragma once
#include "ClientASRDataReq.h"
#include <list>
using namespace std;
typedef list<ClientASRDataReq> ClientASRDataReq_LIST;
typedef ClientASRDataReq_LIST::iterator ClientASRDataReq_LIST_IT;
class ClientASRDataReqList
{
public:
	ClientASRDataReqList(void);
	~ClientASRDataReqList(void);
public:
	bool GetASRReq(ClientASRDataReq & retReq);
	bool AddList(ClientASRDataReq req);
	bool isEmpty();  //ÊÇ·ñÎª¿Õ
private:
	MyMutex _mutex;
	ClientASRDataReq_LIST reqList;
};

