#pragma once
#include "Config.h"
#include "AIUIClient.h"
class ASRManager
{
public:
	ASRManager();
	~ASRManager(void);
private:
	void * m_hASRInstance;
	int m_ASRConnect;
	AIUIClient client;
public:
	static int ASRBeginInitialize();
	static void ASREndUninitialize();
	bool ASRInitConnect(Config config);
	bool ASRStartConnect();
	//针对文字进行语义理解
	int SemanticTxt(const char * szDateBuffer,int nDataBufferLen,const char * szVoxFile);
	int ASRFinishDisconnect();
	int Clean();
	//等待客户端到达指定的状态
	bool waitClientState(int nMilTimeOut,int nState);
};

