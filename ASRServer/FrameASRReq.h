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
	string taskId;  //����ID
	string caller;  //���к���
	string called;  //���к���
	int type;		//�������ͣ�1�����֣�2������
	string content;	//ʶ����������
	string fileName; //Ҫʶ����ļ���
};

