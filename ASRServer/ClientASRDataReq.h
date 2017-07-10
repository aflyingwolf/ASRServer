#pragma once
#include "Client.h"
#include "FrameASRReq.h"
class ClientASRDataReq
{
public:
	ClientASRDataReq(void);
	~ClientASRDataReq(void);
public:
	Client * pClient;
	FrameASRReq req;
};

