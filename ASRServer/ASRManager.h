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
	//合成指定长度的数据到文件
	int ASRToFile(const char * szDateBuffer,int nDataBufferLen,const char * szVoxFile);
	int ASRFinishDisconnect();
	int Clean();
};

