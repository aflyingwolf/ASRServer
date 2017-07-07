#pragma once
#include "frame.h"
#include <string>
using namespace std;
class FrameASRReq :
	public Frame
{
public:
	FrameASRReq(string strHead,string strBody);
	FrameASRReq(void);
	~FrameASRReq(void);
public:
	//根据请求包，生成响应包，空则表示无响应包，或稍后响应包异步生成
	virtual string ProcData();
public:
	string taskId;  //任务ID
	string caller;  //主叫号码
	string called;  //被叫号码
	int type;		//请求类型，1：文字，2：语音
	string content;	//识别文字内容
	string fileName; //要识别的文件名
};

