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
	string taskId;  //任务ID
	string caller;  //主叫号码
	string called;  //被叫号码
	string fileName; //要播放的文件名
	int ret;         //识别结果，0成功，1失败
public:
	//生成类的消息字符串
	virtual string GetFrameString();
};

