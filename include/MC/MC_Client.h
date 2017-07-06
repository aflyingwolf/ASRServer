#pragma once
#include <iostream> 
#include <windows.h>


#define DLL_EXPORT1 __declspec(dllexport)
extern "C" DLL_EXPORT1 bool MC_Init(std::string strAPPName,int heartTimerInterval,std::string strHttpUrl);

#define DLL_EXPORT2 __declspec(dllexport)
extern "C" DLL_EXPORT2 void MC_Stop();

#define DLL_EXPORT3 __declspec(dllexport)
extern "C" DLL_EXPORT3 bool MC_AlarmSend(int alarmLevel, std::string strMsg);

DWORD WINAPI Fun(LPVOID lpParamter);



