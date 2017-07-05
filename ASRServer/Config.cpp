#include "stdafx.h"
#include "Config.h"

Config * Config::_instance=NULL;
Config::Config(void)
{
}


Config::~Config(void)
{
}


void Config::ClearConfig()
{
	if(_instance!=NULL){
		delete _instance;
		_instance=NULL;
	}
}

bool Config::ReadConfig(string cfgFileName,Config * & pConfig)
{
	bool ret=false;
	if(_instance==NULL)
	{
		_instance=new Config();
		_instance->_cfgNodeName="TTSSER";
		_instance->_cfgFileName=cfgFileName;
		
		if(_instance->_cfgFileName.empty())
			ret=false;
		else
		{
			char szTemp[512];
			memset(szTemp,0,sizeof(szTemp));
			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //����
							"TTSServerIp", 
							"",    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str()); 
			_instance->TTSServerIp.assign(szTemp);

			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //����
							"SerialNO", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->SerialNo.assign(szTemp);
	
			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //����
							"ProductName", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->ProductName.assign(szTemp);

			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //����
							"UserName", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->UserName.assign(szTemp);


			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //����
							"CompanyName", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->CompanyName.assign(szTemp);
	
			_instance->Port = GetPrivateProfileInt(_instance->_cfgNodeName.c_str(), //����
										"Port", //����
										1202,    //Ĭ��ֵ
										_instance->_cfgFileName.c_str());  
	
			_instance->ThreadNum = GetPrivateProfileInt(_instance->_cfgNodeName.c_str(), //����
							"THREADNUM", //��ӦIP
							2,    //û�ҵ�����ʱ�ķ���ֵ
							_instance->_cfgFileName.c_str()); 

			GetPrivateProfileString("MONITOR", //����
							"AppName", 
							_T("TTS�ϳɷ����"),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->AppName.assign(szTemp);

			GetPrivateProfileString("MONITOR", //����
							"MonUrl", 
							_T("http://10.100.6.136:7080/httpPost/sendAlarm.action"),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->MonUrl.assign(szTemp);

			_instance->HeartTimerInterval = GetPrivateProfileInt("MONITOR", //����
							"HeartTimerInterval", //��ӦIP
							60,    //û�ҵ�����ʱ�ķ���ֵ
							_instance->_cfgFileName.c_str());

			ret=true;
		}		
	}
	pConfig=_instance;
	return ret;
}