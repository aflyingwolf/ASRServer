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
	string taskId;  //����ID
	string caller;  //���к���
	string called;  //���к���
	string fileName; //Ҫ���ŵ��ļ���
	int ret;         //ʶ������0�ɹ���1ʧ��
public:
	//���������Ϣ�ַ���
	virtual string GetFrameString();
};

