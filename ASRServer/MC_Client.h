#pragma once
#include <iostream> 
#include <windows.h>
typedef enum 
{
	Prompt=101,	//101:һ���쳣,�����ɫ��ʾ
	Warning,	//102:�����쳣,�����ʼ���
	Error,//	//103:�����쳣,����
}AlarmLevel;

bool MC_Init(std::string strAPPName,int heartTimerInterval);

void MC_Stop();

bool MC_AlarmSend(int alarmLevel, std::string strMsg);

bool MC_AlarmError(AlarmLevel alarmLevel, std::string strMsg);

//0:��������
//1:��־����
//2:ͳ������

DWORD WINAPI Fun(LPVOID lpParamter);



