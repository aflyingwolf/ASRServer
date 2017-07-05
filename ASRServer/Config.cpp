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
			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //节名
							"TTSServerIp", 
							"",    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str()); 
			_instance->TTSServerIp.assign(szTemp);

			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //节名
							"SerialNO", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->SerialNo.assign(szTemp);
	
			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //节名
							"ProductName", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->ProductName.assign(szTemp);

			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //节名
							"UserName", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->UserName.assign(szTemp);


			GetPrivateProfileString(_instance->_cfgNodeName.c_str(), //节名
							"CompanyName", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->CompanyName.assign(szTemp);
	
			_instance->Port = GetPrivateProfileInt(_instance->_cfgNodeName.c_str(), //节名
										"Port", //项名
										1202,    //默认值
										_instance->_cfgFileName.c_str());  
	
			_instance->ThreadNum = GetPrivateProfileInt(_instance->_cfgNodeName.c_str(), //节名
							"THREADNUM", //对应IP
							2,    //没找到此项时的返回值
							_instance->_cfgFileName.c_str()); 

			GetPrivateProfileString("MONITOR", //节名
							"AppName", 
							_T("TTS合成服务端"),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->AppName.assign(szTemp);

			GetPrivateProfileString("MONITOR", //节名
							"MonUrl", 
							_T("http://10.100.6.136:7080/httpPost/sendAlarm.action"),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->MonUrl.assign(szTemp);

			_instance->HeartTimerInterval = GetPrivateProfileInt("MONITOR", //节名
							"HeartTimerInterval", //对应IP
							60,    //没找到此项时的返回值
							_instance->_cfgFileName.c_str());

			ret=true;
		}		
	}
	pConfig=_instance;
	return ret;
}