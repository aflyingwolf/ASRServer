#include "stdafx.h"
#include "ASRManager.h"
#include "AIUIClient.h"
//#include <aiui/AIUI.h>
#define ASR_ENABLE 1

ILog * ASRManager::plog=NULL;
ASRManager::ASRManager(void)
{
	
}


ASRManager::~ASRManager(void)
{
}
/******************************************************************************
 *  函数名  :  int BeginInitialize()
 *  描  述  :  //初始化ASR引擎
 ******************************************************************************/
int ASRManager::ASRBeginInitialize(ILog* plog)
{
	ASRManager::plog=plog;
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		/*
		//AIUISetting::setSaveDataLog(true);
		AIUISetting::setAIUIDir(CLIENT_ROOT_DIR);
		AIUISetting::initLogger(LOG_DIR);
		*/
		return 0;
	}
}
/******************************************************************************
 *  函数名  :  void EndUninitialize()
 *  描  述  :  //反初始化
 ******************************************************************************/
void ASRManager::ASREndUninitialize()
{
	if (ASR_ENABLE==0)
		return ;
	else
	{
		
	}
}
/******************************************************************************
 *  函数名  :  int waitClientState()
 *  描  述  :  //等待客户端到达指定的状态
 ******************************************************************************/
bool ASRManager::waitClientState(int nMilTimeOut,int nState)
{
	Sleep(200);
	return true;
}
/******************************************************************************
 *  函数名  :  int waitSemanticResult()
 *  描  述  :  //等待语义识别结果
 ******************************************************************************/
bool ASRManager::waitSemanticResult(int nMilTimeOut)
{
	int nWait=100;
	int nWaitNum=(int)nMilTimeOut/nWait;
	for(int i=0;i<nWaitNum;i++)
	{
		if(0!=client.mnResult)
			Sleep(nWait);
		else
			return true;
	}
	return false;
}
/******************************************************************************
 *  函数名  :  int InitConnect()
 *  描  述  :  //初始化连接
 ******************************************************************************/
bool ASRManager::ASRInitConnect(Config * pConfig,ISemanticResultListener * pTextListener)
{
	if (ASR_ENABLE==0)
		return true;
	else
	{
		client.m_WriteLog.init(pConfig->LogPath.c_str(),LOG_NAME_AIUI_CLIENT);
		client.init(pTextListener,pConfig->dialog_url);
		client.createAgent(pConfig->appId);
		return true;
	}
}


/******************************************************************************
 *  函数名  :  int StartConnect()
 *  描  述  :  //连接服务器，并保存环境句柄
 ******************************************************************************/
bool ASRManager::ASRStartConnect()
{
	if (ASR_ENABLE==0)
		return true;
	else
	{
		client.wakeup();
		return waitClientState(AIUI_EVENT_WAIT_TIMEOUT,AIUIConstant::STATE_WORKING);
	}
}

/******************************************************************************
 *  函数名  :  SemanticTxt(string content)
 *  描  述  :  语义识别
 ******************************************************************************/
int ASRManager::SemanticTxt(string content)
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		client.writeText(content);
		return 0;
	}
}

/******************************************************************************
 *  函数名  :  SemanticVox(string fileName)
 *  描  述  :  语义识别
 ******************************************************************************/
int ASRManager::SemanticVox(string fileName)
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		client.write(fileName);
		return 0;
	}
}
/******************************************************************************
 *  函数名  :  int FinishDisconnect()
 *  描  述  :  //断开连接
 ******************************************************************************/
int ASRManager::ASRFinishDisconnect()
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		client.destory();
		return 0;
	}
}
/******************************************************************************
 *  函数名  :  int Clean()
 *  描  述  :  //清除合成缓冲区
 ******************************************************************************/
int ASRManager::Clean()
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		return 0;
	}
}
