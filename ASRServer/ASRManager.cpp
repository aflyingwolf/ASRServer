#include "stdafx.h"
#include "ASRManager.h"
#include "AIUIClient.h"
//#include <aiui/AIUI.h>
#define ASR_ENABLE 1

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
int ASRManager::ASRBeginInitialize()
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		AIUISetting::setSaveDataLog(true);
		AIUISetting::setAIUIDir(CLIENT_ROOT_DIR);
		AIUISetting::initLogger(LOG_DIR);
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
		;//ASRUninitialize();
}
/******************************************************************************
 *  ������  :  int waitClientState()
 *  ��  ��  :  //�ȴ��ͻ��˵���ָ����״̬
 ******************************************************************************/
bool ASRManager::waitClientState(int nMilTimeOut,int nState)
{
	int nWait=100;
	int nWaitNum=(int)nMilTimeOut/nWait;
	for(int i=0;i<nWaitNum;i++)
	{
		if(nState!=client.mnState)
			Sleep(nWait);
		else
			return true;
	}
	return false;
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
bool ASRManager::ASRInitConnect(Config config)
{
	if (ASR_ENABLE==0)
		return true;
	else
	{
		m_hASRInstance=NULL;
		client.createAgent();
		return waitClientState(AIUI_EVENT_WAIT_TIMEOUT,AIUIConstant::STATE_IDLE);
	}
}


/******************************************************************************
 *  ������  :  int StartConnect()
 *  ��  ��  :  //���ӷ������������滷�����
 ******************************************************************************/
bool ASRManager::ASRStartConnect()
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		client.wakeup();
		return waitClientState(AIUI_EVENT_WAIT_TIMEOUT,AIUIConstant::STATE_WORKING);
	}
}

/******************************************************************************
 *  ������  :  SemanticTxt(string content,string & result)
 *  ��  ��  :  ����ʶ��
 ******************************************************************************/
int ASRManager::SemanticTxt(string content,string & result)
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		client.writeText();
		int ret=waitSemanticResult(AIUI_EVENT_WAIT_TIMEOUT);
		if(ret==0)//�ɹ�
		{
			result=client.resultStr;
		}
		return ret;
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