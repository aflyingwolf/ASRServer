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
	//等待客户端到达指定的状态
	bool waitClientState(int nMilTimeOut,int nState);
	//等待识别结果
	bool waitSemanticResult(int nMilTimeOut);
public:
	static int ASRBeginInitialize(ILog* plog);
	static void ASREndUninitialize();
	bool ASRInitConnect(Config* pConfig,ISemanticResultListener * pTextListener);
	bool ASRStartConnect();
	//针对文字进行语义理解
	int SemanticTxt(string content);
	//针对语音文件进行语义理解
	int SemanticVox(string fileName);
	int ASRFinishDisconnect();
	int Clean();
};

