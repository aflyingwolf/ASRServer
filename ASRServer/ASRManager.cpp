#include "stdafx.h"
#include "ASRManager.h"
#define ASR_ENABLE 1

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
int ASRManager::ASRBeginInitialize()
{
	if (ASR_ENABLE==0)
		return 0;
	else 
		return 0;//ASRInitializeEx(NULL,NULL);
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
		;//ASRUninitialize();
}
/******************************************************************************
 *  函数名  :  int InitConnect()
 *  描  述  :  //初始化连接
 ******************************************************************************/
bool ASRManager::ASRInitConnect(Config config)
{
	if (ASR_ENABLE==0)
		return true;
	else
	{
		m_hASRInstance=NULL;
		/*
		memset(&m_ASRConnect, 0, sizeof(ASRConnectStruct));
		m_ASRConnect.dwSDKVersion = IFLYASR_SDK_VER;
		strcpy(m_ASRConnect.szCompanyName, config.CompanyName.c_str());
		strcpy(m_ASRConnect.szUserName, config.UserName.c_str());
		strcpy(m_ASRConnect.szProductName,config.ProductName.c_str());
		strcpy(m_ASRConnect.szSerialNumber, config.SerialNo.c_str());
		// Comment: Need network runtime library's support
		strcpy(m_ASRConnect.szASRServerIP,config.ASRServerIp.c_str());
		*/
		return true;
	}
}


/******************************************************************************
 *  函数名  :  int StartConnect()
 *  描  述  :  //连接服务器，并保存环境句柄
 ******************************************************************************/
int ASRManager::ASRStartConnect()
{
	if (ASR_ENABLE==0)
		return 0;
	else
	{
		/*
		m_hASRInstance = ASRConnect(&m_ASRConnect);

		if( NULL == m_hASRInstance)
		{
			return ASRGETERRCODE(m_ASRConnect.dwErrorCode);
		}
		return ASRERR_OK;
		*/
		return 0;
	}
}

/******************************************************************************
 *  函数名  :  ASRToFile(char * szDateBuffer,int nDataBufferLen,char * szVoxFile)
 *  描  述  :  //合成语音数据
 ******************************************************************************/
int ASRManager::ASRToFile(const char * szDateBuffer,int nDataBufferLen,const char * szVoxFile)
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
 *  函数名  :  int FinishDisconnect()
 *  描  述  :  //断开连接
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
 *  函数名  :  int Clean()
 *  描  述  :  //清除合成缓冲区
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