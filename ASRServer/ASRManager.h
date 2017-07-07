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
private:
	//�ȴ��ͻ��˵���ָ����״̬
	bool waitClientState(int nMilTimeOut,int nState);
	//�ȴ�ʶ����
	bool waitSemanticResult(int nMilTimeOut);
public:
	static int ASRBeginInitialize();
	static void ASREndUninitialize();
	bool ASRInitConnect(Config config);
	bool ASRStartConnect();
	//������ֽ����������
	int SemanticTxt(string content,string & result);
	int ASRFinishDisconnect();
	int Clean();
};

