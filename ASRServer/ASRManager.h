#pragma once
#include "Config.h"
class ASRManager
{
public:
	ASRManager();
	~ASRManager(void);
private:
	void * m_hASRInstance;
	int m_ASRConnect;
public:
	static int ASRBeginInitialize();
	static void ASREndUninitialize();
	bool ASRInitConnect(Config config);
	int ASRStartConnect();
	//�ϳ�ָ�����ȵ����ݵ��ļ�
	int ASRToFile(const char * szDateBuffer,int nDataBufferLen,const char * szVoxFile);
	int ASRFinishDisconnect();
	int Clean();
};

