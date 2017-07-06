#pragma once
#include <string>
using namespace std;
class Config
{
public:
	~Config(void);
	//��ȡ�����ļ�
	static bool ReadConfig(const string cfgFileName,Config * & pConfig);
	static void ClearConfig();
private:
	 Config(void);
	 string _cfgFileName;
	 string _cfgNodeName;  //���õĽ���
	 static Config * _instance;
public:
	 string ASRServerIp;
	 string SerialNo;
	 string ProductName;
	 int Port;
	 string UserName;
	 string CompanyName;
	 int ThreadNum;
	 string AppName;  //Ӧ�ù��ܳ�������
	 string MonUrl;   //���URL
	 int HeartTimerInterval;  //����ʱ����
};

