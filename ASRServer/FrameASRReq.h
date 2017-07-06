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
	string chan;
	string chanState;
	string caller;
	int fileNum;  
	string ASRData;  //要合成的数据
	string fileName; //要合成的文件名
	string code;	//业务代码
};

