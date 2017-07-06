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
	 string _cfgNodeName;  //配置的节名
	 static Config * _instance;
public:
	 string ASRServerIp;
	 string SerialNo;
	 string ProductName;
	 int Port;
	 string UserName;
	 string CompanyName;
	 int ThreadNum;
	 string AppName;  //应用功能程序名称
	 string MonUrl;   //监控URL
	 int HeartTimerInterval;  //心跳时间间隔
};

