#pragma once
#include "frame.h"
#include <string>
using namespace std;
/**
����һ��Frame�࣬������Ϊ�գ��������͸���
*/
class FrameNull :
	public Frame
{
public:
	FrameNull(void);
	~FrameNull(void);
public:
	//���������Ϣ�ַ���
	virtual std::string GetFrameString();
};

