#pragma once
#include <string>
using namespace std;
class Config
{
public:
	~Config(void);
	//读取配置文件
	static bool ReadConfig(const string cfgFileName,Config * & pConfig);
	static void ClearConfig();
private:
	Config(void);
	string _cfgFileName;
	static Config * _instance;
public:
	string DataPath;
	string LogPath;
	string ASRServerIp;
	int Port;
	string AppName;  //应用功能程序名称
	string MonUrl;   //监控URL
	int HeartTimerInterval;  //心跳时间间隔
	string appId;	 //APPID
	string dialog_url;	//对话服务的URL
};

