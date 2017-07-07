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
 *  ������  :  ASRToFile(char * szDateBuffer,int nDataBufferLen,char * szVoxFile)
 *  ��  ��  :  //�ϳ���������
 ******************************************************************************/
int ASRManager::ASRTxt(const char * szDateBuffer,int nDataBufferLen,const char * szVoxFile)
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		return 0;
		/*
		if( NULL == m_hASRInstance)
			return ASRERR_FALSE;
		
		int nAudioHead;
		int nAudioData;
		int nCode = ASR_CP_AUTO;
		int nREADNUMBER=ASR_RN_DIGIT;
		int nSpeech=-50;
		int nVolume=2;
		int nPitch=3;
		int nRet;
		//int nAudioData = ASR_ADF_ALAW8K1C;

		
		

		ASRData ASRData;
		memset(&ASRData,0,sizeof(ASRData));
		ASRData.dwServiceID=m_ASRConnect.dwServiceID;
		ASRData.dwInBufSize=nDataBufferLen;
		ASRData.szInBuf=const_cast<char *>(szDateBuffer);

		nRet = ASRSynthText2File(m_hASRInstance, 
			&ASRData,szVoxFile,NULL, false,NULL);
		if ( nRet != ASRERR_OK )
		{
			;
		}
		return nRet;
		*/
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
		/*
		if(m_hASRInstance!=NULL)
		{
			return ASRDisconnect(m_hASRInstance);
			
		}
		else
			return ASRERR_FALSE;
		*/
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
		/*
		if(m_hASRInstance!=NULL)
		{
			return ASRClean(m_hASRInstance);
		}
		else return 0;
		*/
		return 0;
	}
}