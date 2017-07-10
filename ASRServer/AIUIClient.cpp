#include "stdafx.h"
#include "AIUIClient.h"
#include "json/json.h"
#include "string_convert.h"
bool WriteAudioThread::threadLoop()
{
	char log[LOG_MAX_LENGTH];
	try
	{
		char audio[1279];
		int len = mFileHelper->read(audio, 1279);

		if (len > 0)
		{
			Buffer* buffer = Buffer::alloc(len);
			memcpy(buffer->data(), audio, len);

			/* param需要UTF-8编码的字符串 */
			IAIUIMessage * writeMsg = IAIUIMessage::create(AIUIConstant::CMD_WRITE,
				0, 0, "data_type=audio,sample_rate=16000", buffer);

			if (NULL != mAgent)
			{
				mAgent->sendMessage(writeMsg);
			}		
			writeMsg->destroy();
			Sleep(10); // 模拟10ms的说话间隔
		} else {
			IAIUIMessage * stopWrite = IAIUIMessage::create(AIUIConstant::CMD_STOP_WRITE,
					0, 0, "data_type=audio,sample_rate=16000");

			if (NULL != mAgent)
			{
				mAgent->sendMessage(stopWrite);
			}
			stopWrite->destroy();

			mFileHelper->closeReadFile();
			mRun = false;
		}
	}
	catch(...)
	{
		mRun=false;
		try
		{
			sprintf(log,"WriteAudioThread::threadLoop Error");
			m_WriteLog.WriteLog(Log::ERROR_INFO,log);
		}
		catch(...)
		{
			;
		}
	}

	return mRun;
}


unsigned int __stdcall WriteAudioThread::WriteProc(void * paramptr)
{
	WriteAudioThread * self = (WriteAudioThread *)paramptr;

	while (1) {
		if (! self->threadLoop())
			break;
	}
		
	return 0;
}

WriteAudioThread::WriteAudioThread(IAIUIAgent* agent, const string& audioPath,Log writeLog):
mAgent(agent), mAudioPath(audioPath),mRun(true), mFileHelper(NULL),m_WriteLog(writeLog)
	,thread_hdl_(NULL)
{
	mFileHelper = new FileUtil::DataFileHelper("");
	mFileHelper->openReadFile(mAudioPath, false);
}

WriteAudioThread::~WriteAudioThread()
{
	
}

void WriteAudioThread::stopRun()
{
	mRun = false;
	WaitForSingleObject(thread_hdl_, INFINITE);
	thread_hdl_ = NULL;
}

bool WriteAudioThread::run()
{
	if (thread_hdl_ == NULL) {
		thread_hdl_ = (HANDLE)_beginthreadex(NULL, 0, WriteProc, (LPVOID) this, 0,  &thread_id_);
		return true;
	}

	return false;
}

void AIUIClient::onEvent(IAIUIEvent& event)
{
	try
	{
		char log[LOG_MAX_LENGTH];
		sprintf(log,"onEvent getEventType:%d",event.getEventType());
		m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
		switch (event.getEventType()) {
		case AIUIConstant::EVENT_STATE:
			{
				sprintf(log,"onEvent  EVENT_STATE:%d",event.getArg1());
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
				this->mnState=event.getArg1();
				switch (event.getArg1()) {
				case AIUIConstant::STATE_IDLE:
					{
						sprintf(log,"onEvent  EVENT_STATE:STATE_IDLE");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					} break;

				case AIUIConstant::STATE_READY:
					{
						sprintf(log,"onEvent  EVENT_STATE:STATE_READY");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					} break;

				case AIUIConstant::STATE_WORKING:
					{
						sprintf(log,"onEvent  EVENT_STATE:STATE_WORKING");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					} break;
				}
			} break;

		case AIUIConstant::EVENT_WAKEUP:
			{
				sprintf(log,"onEvent EVENT_WAKEUP:%s",event.getInfo());
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
			} break;

		case AIUIConstant::EVENT_SLEEP:
			{
				sprintf(log,"onEvent EVENT_SLEEP:%s",event.getInfo());
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
			} break;

		case AIUIConstant::EVENT_VAD:
			{
				switch (event.getArg1()) {
				case AIUIConstant::VAD_BOS:
					{
						sprintf(log,"onEvent EVENT_VAD:%s","BOS");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					} break;

				case AIUIConstant::VAD_EOS:
					{
						sprintf(log,"onEvent EVENT_VAD:%s","EOS");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					} break;

				case AIUIConstant::VAD_VOL:
					{
						sprintf(log,"onEvent EVENT_VAD:%s","VOL");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					} break;
				}
			} break;

		case AIUIConstant::EVENT_RESULT:
			{
				sprintf(log,"onEvent EVENT_RESULT");
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
				
				this->mnState=1;
				using namespace VA;
				Json::Value bizParamJson;
				Json::Reader reader;
				
				if (!reader.parse(event.getInfo(), bizParamJson, false)) {
					sprintf(log,"onEvent EVENT_RESULT parse error:%s",event.getInfo());
					m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					break;
				}
				Json::Value data = (bizParamJson["data"])[0];
				Json::Value params = data["params"];
				Json::Value content = (data["content"])[0];
				string sub = params["sub"].asString();
				sprintf(log,"onEvent EVENT_RESULT :%s",sub.c_str());
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);

				if (sub=="iat")
				{
					Json::Value empty;
					Json::Value contentId = content.get("cnt_id", empty);

					if (contentId.empty())
					{
						sprintf(log,"onEvent iat Content Id is empty");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
						break;
					}

					string cnt_id = contentId.asString();
					Buffer* buffer = event.getData()->getBinary(cnt_id.c_str());
					string resultStr;

					if (NULL != buffer)
					{
						resultStr = string((char*)buffer->data());

						sprintf(log,"onEvent iat %s:",resultStr.c_str());
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
						string utf8Str=this->getListenTextAnswer(resultStr);
						
						sprintf(log,"onEvent iat utf8: %s:",utf8Str.c_str());
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
					}
				}
				else if (sub == "nlp")
				{
					Json::Value empty;
					Json::Value contentId = content.get("cnt_id", empty);

					if (contentId.empty())
					{
						sprintf(log,"onEvent nlp Content Id is empty");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
						break;
					}

					string cnt_id = contentId.asString();
					Buffer* buffer = event.getData()->getBinary(cnt_id.c_str());
					string resultStr;

					if (NULL != buffer)
					{
						resultStr = string((char*)buffer->data());
						sprintf(log,"onEvent nlp %s:",resultStr.c_str());
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
						//解析JSON
						this->mnResult=0;
						this->resultStr=getSemanticAnswer(resultStr);
						SementicResultTextEvent textEvent(this->resultStr);
						textEvent.pListener=this->pListener;
						if(this->pListener!=NULL)
							this->pListener->onEvent(&textEvent);
					}
				}

			} break;

		case AIUIConstant::EVENT_ERROR:
			{
				sprintf(log,"onEvent EVENT_ERROR %s:",event.getArg1());
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
			} break;
		case AIUIConstant::EVENT_CMD_RETURN:
			{
				sprintf(log,"onEvent EVENT_CMD_RETURN CMD=%d,, Return code=%d, Info=%s",event.getArg1(),event.getArg2(),event.getInfo());
				m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
			} break;
		}
	}
	catch(...)
	{
		m_WriteLog.WriteLog(Log::ERROR_INFO,"AIUIClient::onEvent Error");
	}
}
/**
根据语义识别文字JSON结果返回对应的回答
**/
string AIUIClient::getListenTextAnswer(string strListenTextJson)
{
	char log[2048];
	string answer="";
	using namespace VA;
	Json::Value bizJson;
	Json::Reader reader;
	
	if (!reader.parse(strListenTextJson, bizJson, false)) {
		sprintf(log,"getLinsenTextAnswer parse error:%s",strListenTextJson.c_str());
		m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
		answer="";
	}
	//听写结果
	Json::Value ws=((bizJson["text"])["ws"]);
	for(Json::Value::iterator it=ws.begin();it!=ws.end();it++)
	{
		Json::Value val=*it;
		string word=((val["cw"])[0])["w"].asString();
		string wordUtf8=string_convert::utfs2s(word);
		sprintf(log,"getLinsenTextAnswer word:%s,utf8:%s",word.c_str(),wordUtf8.c_str());
		m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
		answer=answer+wordUtf8;
	}
	return answer;
}
/**
根据语义识别JSON结果返回对应的回答
**/
string AIUIClient::getSemanticAnswer(string strSemanticJson)
{
	char log[LOG_MAX_LENGTH];
	string answer;
	using namespace VA;
	Json::Value bizJson;
	Json::Reader reader;
	
	if (!reader.parse(strSemanticJson, bizJson, false)) {
		sprintf(log,"getSemanticAnswer parse error:%s",strSemanticJson);
		m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
		answer="";
	}
	Json::Value answerRc=(bizJson["intent"])["rc"];
	int nRc=answerRc.asInt();
	if(nRc==0)//成功
	{
		//判断语义识别结果
		Json::Value answerValue = ((bizJson["intent"])["answer"])["text"];
		answer=answerValue.asString();
		string answerUtf8=string_convert::utfs2s(answer);
		answer=answerUtf8;
	}
	else
	{
		answer="";
	}
	return answer;
}
AIUIClient::AIUIClient() : agent(NULL), writeThread(NULL),mnResult(-1)
{

}

AIUIClient::~AIUIClient() 
{
	if (agent) {
		agent->destroy();
		agent = NULL;
	}
}

void AIUIClient::createAgent(string appId)
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient createAgent begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	string params="appid="+appId;
	ISpeechUtility::createSingleInstance("", "",
		params.c_str());

	string paramStr = FileUtil::readFileAsString(CFG_FILE_PATH);
	agent = IAIUIAgent::createAgent(paramStr.c_str(), this);
	sprintf(log,"AIUIClient createAgent end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}

void AIUIClient::wakeup()
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient wakeup begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (NULL != agent)
	{
		IAIUIMessage * wakeupMsg = IAIUIMessage::create(AIUIConstant::CMD_WAKEUP);
		agent->sendMessage(wakeupMsg);
		wakeupMsg->destroy();
	}
	sprintf(log,"AIUIClient wakeup end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}

void AIUIClient::start()
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient start begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (NULL != agent)
	{
		IAIUIMessage * startMsg = IAIUIMessage::create (AIUIConstant::CMD_START);
		agent->sendMessage(startMsg);
		startMsg->destroy();
	}
	sprintf(log,"AIUIClient start end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}

void AIUIClient::stop()
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient stop begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (NULL != agent)
	{
		IAIUIMessage *stopMsg = IAIUIMessage::create (AIUIConstant::CMD_STOP);
		agent->sendMessage(stopMsg);
		stopMsg->destroy();
	}
	sprintf(log,"AIUIClient stop end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}


void AIUIClient::write(string file)
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient write begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (agent == NULL)
		return;

	if (writeThread != NULL) {
		stopWriteThread(); 
	}
	if (writeThread == NULL) {
		writeThread = new WriteAudioThread(agent, file,this->m_WriteLog);
		writeThread->run();
	}	
	sprintf(log,"AIUIClient write end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}



void AIUIClient::stopWriteThread()
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient stopWriteThread begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (writeThread) {
		writeThread->stopRun();
		delete writeThread;
		writeThread = NULL;
	}
	sprintf(log,"AIUIClient stopWriteThread end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}

void AIUIClient::reset()
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient reset begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (NULL != agent)
	{
		IAIUIMessage * resetMsg = IAIUIMessage::create(AIUIConstant::CMD_RESET);
		agent->sendMessage(resetMsg);
		resetMsg->destroy();
	}
	sprintf(log,"AIUIClient reset end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}
void AIUIClient::writeText(string text)
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient writeText begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	if (NULL != agent)
	{
		mnResult=-1;
		text=string_convert::s2utfs(text);
		// textData内存会在Message在内部处理完后自动release掉
		Buffer* textData = Buffer::alloc(text.length());
		text.copy((char*) textData->data(), text.length());

		IAIUIMessage * writeMsg = IAIUIMessage::create(AIUIConstant::CMD_WRITE,
			0,0, "data_type=text", textData);	

		agent->sendMessage(writeMsg);
		writeMsg->destroy();		
	}
	sprintf(log,"AIUIClient writeText end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}

void AIUIClient::destory()
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient destory begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	stopWriteThread();
	if (NULL != agent)
	{
		agent->destroy();
		agent = NULL;
	}
	sprintf(log,"AIUIClient destory end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}

void AIUIClient::init(ISemanticResultListener * pListener)
{
	char log[LOG_MAX_LENGTH];
	sprintf(log,"AIUIClient init begin");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
	//		AIUISetting::setSaveDataLog(true);
	AIUISetting::setAIUIDir(CLIENT_ROOT_DIR);
	AIUISetting::initLogger(LOG_DIR);
	this->pListener=pListener;
	sprintf(log,"AIUIClient init end");
	m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
}