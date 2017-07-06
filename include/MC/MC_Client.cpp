#include "MC_Client.h"  
#include "MC_HttpReq.h"
#include "MC_Tools.h"
#include "MC_DllData.h"
#include "MC_HeartThread.h"
#include <iostream>  

bool MC_Init(std::string strAPPName,int heartTimerInterval,std::string strHttpUrl)
{
	DllData::GetInstance()->SetStrUrl(strHttpUrl);

	DllData::GetInstance()->SetStrAppName(strAPPName);

	DllData::GetInstance()->SetHeartTimer(heartTimerInterval);
	CStringW Url=CA2W(strHttpUrl.c_str());
	char XmlMsg[1024]={0};
	sprintf(XmlMsg,"xml=%s",CreatInitXML(strAPPName).c_str());
	//CreatInitXML(strAPPName).c_str()
	CStringW Req=CA2W(XmlMsg);

	BOOL BRet=WinHttp::GetInstance()->HttpRequest(Url,Req);//Http 请求

	if (BRet==0)//Http接口调用成
	{
		if (heartTimerInterval>0)//有心跳
		{
			ClientThread * CThread=new ClientThread(120);
			CThread->Start();
			DllData::GetInstance()->SetThread(CThread);
		}
		else
		{
			//没有心跳
		}
		return true;
	}
	else
	{
		return false;
	}
}
void MC_Stop()
{	
	if (DllData::GetInstance()->GetThread()!=NULL)
	{
		DllData::GetInstance()->GetThread()->Stop();
	}
	else
	{

	}
}
bool MC_AlarmSend(int alarmLevel, std::string strMsg)
{
	char XmlMsg[1024]={0};

	sprintf(XmlMsg,"xml=%s",CreatWarXML(DllData::GetInstance()->GetStrAppName(),0,strMsg).c_str());

	CStringW APPMsg=CA2W(XmlMsg);
	
	CStringW CstrUrlW=CA2W(DllData::GetInstance()->GetStrUrl().c_str());

	BOOL BRet=WinHttp::GetInstance()->HttpRequest(CstrUrlW,APPMsg);	

	if (BRet==0)//
	{
		return true;
	}
	else
	{
		return false;
	}
}




