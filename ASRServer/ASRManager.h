#pragma once
#include "Config.h"
#include "AIUIClient.h"
#include "ClientASRDataReq.h"
#include "Log.h"
class ASRManager
{
public:
	ASRManager();
	~ASRManager(void);
private:
	AIUIClient client;
	static ILog * plog;
private:
	//�ȴ��ͻ��˵���ָ����״̬
	bool waitClientState(int nMilTimeOut,int nState);
	//�ȴ�ʶ����
	bool waitSemanticResult(int nMilTimeOut);
public:
	static int ASRBeginInitialize(ILog* plog);
	static void ASREndUninitialize();
	bool ASRInitConnect(Config* pConfig,ISemanticResultListener * pTextListener);
	bool ASRStartConnect();
	//������ֽ����������
	int SemanticTxt(string content);
	//��������ļ������������
	int SemanticVox(string fileName);
	int ASRFinishDisconnect();
	int Clean();
};

