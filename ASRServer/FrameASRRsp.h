#pragma once
#include "frame.h"
#include <string>
using namespace std;
class FrameASRRsp :
	public Frame
{
public:
	FrameASRRsp(void);
	~FrameASRRsp(void);
public:
	string chan;
	string chanState;
	string caller;
	int fileNum; 
	string fileName;
public:
	//生成类的消息字符串
	virtual string GetFrameString();
};

