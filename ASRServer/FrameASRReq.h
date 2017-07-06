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
	//�����������������Ӧ���������ʾ����Ӧ�������Ժ���Ӧ���첽����
	virtual string ProcData();
public:
	string chan;
	string chanState;
	string caller;
	int fileNum;  
	string ASRData;  //Ҫ�ϳɵ�����
	string fileName; //Ҫ�ϳɵ��ļ���
	string code;	//ҵ�����
};

