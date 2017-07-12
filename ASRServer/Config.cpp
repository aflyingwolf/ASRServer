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
		_instance->_cfgFileName=cfgFileName;
		if(_instance->_cfgFileName.empty())
			ret=false;
		else
		{
			char szTemp[512];
			memset(szTemp,0,sizeof(szTemp));

			GetPrivateProfileString("PATH", //节名
							"DataPath", 
							"G:\\USER",    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str()); 
			_instance->DataPath.assign(szTemp);

			GetPrivateProfileString("PATH", //节名
							"LogPath", 
							"D:\\ccs\\log",    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str()); 
			_instance->LogPath.assign(szTemp);

			GetPrivateProfileString("ASRSER", //节名
							"ASRServerIp", 
							"",    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str()); 
			_instance->ASRServerIp.assign(szTemp);

			_instance->Port = GetPrivateProfileInt("ASRSER", //节名
										"Port", //项名
										1202,    //默认值
										_instance->_cfgFileName.c_str());  
	
	
			GetPrivateProfileString("MONITOR", //节名
							"AppName", 
							_T("ASR服务端"),    
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
			
			GetPrivateProfileString("AIUI", //节名
							"appId", 
							_T("595ce74c"),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->appId.assign(szTemp);

			GetPrivateProfileString("DIALOG", //节名
							"URL", 
							_T(""),    
							szTemp, 
							sizeof(szTemp), 
							_instance->_cfgFileName.c_str());
			_instance->dialog_url.assign(szTemp);

			ret=true;
		}		
	}
	pConfig=_instance;
	return ret;
}