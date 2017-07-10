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
 *  ������  :  int BeginInitialize()
 *  ��  ��  :  //��ʼ��ASR����
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
 *  ������  :  void EndUninitialize()
 *  ��  ��  :  //����ʼ��
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
 *  ������  :  int waitClientState()
 *  ��  ��  :  //�ȴ��ͻ��˵���ָ����״̬
 ******************************************************************************/
bool ASRManager::waitClientState(int nMilTimeOut,int nState)
{
	Sleep(200);
	return true;
}
/******************************************************************************
 *  ������  :  int waitSemanticResult()
 *  ��  ��  :  //�ȴ�����ʶ����
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
 *  ������  :  int InitConnect()
 *  ��  ��  :  //��ʼ������
 ******************************************************************************/
bool ASRManager::ASRInitConnect(Config * pConfig,ISemanticResultListener * pTextListener)
{
	if (ASR_ENABLE==0)
		return true;
	else
	{
		client.m_WriteLog.init(pConfig->LogPath.c_str(),LOG_NAME_AIUI_CLIENT);
		client.init(pTextListener);
		client.createAgent(pConfig->appId);
		return true;
	}
}


/******************************************************************************
 *  ������  :  int StartConnect()
 *  ��  ��  :  //���ӷ������������滷�����
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
 *  ������  :  SemanticTxt(string content)
 *  ��  ��  :  ����ʶ��
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
 *  ������  :  SemanticVox(string fileName)
 *  ��  ��  :  ����ʶ��
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
 *  ������  :  int FinishDisconnect()
 *  ��  ��  :  //�Ͽ�����
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
 *  ������  :  int Clean()
 *  ��  ��  :  //����ϳɻ�����
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
