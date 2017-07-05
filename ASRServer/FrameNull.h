#pragma once
#include "frame.h"
#include <string>
using namespace std;
/**
代表一个Frame类，但该类为空，不允许发送该侦
*/
class FrameNull :
	public Frame
{
public:
	FrameNull(void);
	~FrameNull(void);
public:
	//生成类的消息字符串
	virtual std::string GetFrameString();
};

