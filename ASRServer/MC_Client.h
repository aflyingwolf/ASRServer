#pragma once
#include <iostream> 
#include <windows.h>
typedef enum 
{
	Prompt=101,	//101:一级异常,界面黄色显示
	Warning,	//102:二级异常,发送邮件等
	Error,//	//103:三级异常,短信
}AlarmLevel;

bool MC_Init(std::string strAPPName,int heartTimerInterval);

void MC_Stop();

bool MC_AlarmSend(int alarmLevel, std::string strMsg);

bool MC_AlarmError(AlarmLevel alarmLevel, std::string strMsg);

//0:心跳类型
//1:日志类型
//2:统计类型

DWORD WINAPI Fun(LPVOID lpParamter);



