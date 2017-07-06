#pragma once
#include "frame.h"
class FrameConnectTest :
	public Frame
{
public:
	FrameConnectTest(std::string strHead,std::string strBody);
	FrameConnectTest(void);
	~FrameConnectTest(void);
public:
	virtual string ProcData();
};

