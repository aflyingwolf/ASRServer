#pragma once
#include "MC_HeartThread.h"
#include <iostream> 
#include <windows.h>

class DllData
{
public:
	static DllData* GetInstance();
	void SetStrUrl(std::string strUrl){m_strUrl=strUrl;}
	void SetStrAppName(std::string strAppName){m_strAppName=strAppName;}
	void SetHeartTimer(int nHearTimer){m_nHeartTimer=nHearTimer;}
	void SetThread(ClientThread* hThread){m_hThread=hThread;}
	std::string GetStrUrl() {return m_strUrl;}
	std::string GetStrAppName() {return m_strAppName;}
	int GetHearTimer() {return m_nHeartTimer;}
	ClientThread* GetThread() {return m_hThread;}

private:
	DllData();

	~DllData();

private:
	std::string m_strUrl;

	std::string m_strAppName;

	int m_nHeartTimer;

	ClientThread* m_hThread;

	static DllData*	m_pInstance;

};
