/*
* AIUIAgentClient.h
*
*  Created on: 2017年3月9日
*      Author: hj
*/

#ifndef AIUIAGENTCLIENT_H_
#define AIUIAGENTCLIENT_H_

#include <aiui/AIUI.h>


#include <string>
#include <iostream>

#include "FileUtil.h"
#include "Log.h"
#include "SemanticResultListener.h"
#include "DialogAnswer.h"
#include <list>
#ifdef _MSC_VER
#include <windows.h>
#include <process.h>
#define CLIENT_ROOT_DIR ".\\AIUI"
#define CFG_FILE_PATH CLIENT_ROOT_DIR##"\\cfg\\aiui.cfg"
#define CLIENT_AUDIO_PATH CLIENT_ROOT_DIR##"\\audio\\test.pcm"
#define LOG_DIR CLIENT_ROOT_DIR##"\\log"
#else
#include <unistd.h>
#define CLIENT_ROOT_DIR "/home/anxingg/AIUI"
#define CFG_FILE_PATH CLIENT_ROOT_DIR##"/cfg/aiui.cfg"
#define CLIENT_AUDIO_PATH CLIENT_ROOT_DIR##"/test/audio/test.pcm"
#define LOG_DIR CLIENT_ROOT_DIR##"/log"
#endif

#pragma comment(lib, "HttpLib.lib")

#define LOG_NAME_AIUI_CLIENT "AIUIClient"
//等待AIUI响应的时间间隔
#define AIUI_EVENT_WAIT_TIMEOUT 6000
//语言文件频率
#define AIUI_VOX_RATE 16000
using namespace aiui;
using namespace std;


class WriteAudioThread 
{
private:
	IAIUIAgent* mAgent;

	string mAudioPath;

	bool mRun;

	Log m_WriteLog;

	FileUtil::DataFileHelper* mFileHelper;

	HANDLE  thread_hdl_;
	unsigned int  thread_id_;


private:
	bool threadLoop();

	static unsigned int __stdcall WriteProc(void * paramptr) ;


public:
	WriteAudioThread(IAIUIAgent* agent, const string& audioPath,Log writeLog);

	~WriteAudioThread();

	void stopRun();

	bool run();

};


class AIUIClient: public IAIUIListener
{
public:
	int mnState;   //AIUI状态
	int mnResult;  //ASR状态成功0/调用中-1/失败1
	string resultStr;	//ASR返回内容
	//日志类
	Log m_WriteLog;
private:
	string dialog_url;	//对话服务的URL
	IAIUIAgent* agent;
	WriteAudioThread * writeThread;
	ISemanticResultListener * pListener;
private:
	string getSemanticAnswer(string strSemanticJson);
	list<string> getListenTextAnswer(string strListenTextJson);
	DialogAnswer getDialogAnswer(string question);
public:
	AIUIClient() ;

	~AIUIClient();
public:
	void init(ISemanticResultListener * pListener,string dialog_url);
	void createAgent(string appId);
	void wakeup();
	void start();
	void stop();
	void write(string file);
	void stopWriteThread();
	void reset();
	void writeText(string text);
	void sync();
	void destory();
private:
	void onEvent(IAIUIEvent& event);	
};

#endif /* AIUIAGENTCLIENT_H_ */
