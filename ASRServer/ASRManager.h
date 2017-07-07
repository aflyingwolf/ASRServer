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
	//������ֽ����������
	int SemanticTxt(const char * szDateBuffer,int nDataBufferLen,const char * szVoxFile);
	int ASRFinishDisconnect();
	int Clean();
	//�ȴ��ͻ��˵���ָ����״̬
	bool waitClientState(int nMilTimeOut,int nState);
};

