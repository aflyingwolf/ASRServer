#pragma once
#include "Client.h"
#include "FrameASRReq.h"
#include "MyMutex.h"
class ClientASRDataReq
{
public:
	ClientASRDataReq(void);
	~ClientASRDataReq(void);
public:
	SOCKET Client;
	FrameASRReq req;
};

