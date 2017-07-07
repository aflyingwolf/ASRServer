#include "stdafx.h"
#include "AIUIClient.h"
#include "json/json.h"

bool WriteAudioThread::threadLoop()
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
		if (mRepeat)
		{
			mFileHelper->rewindReadFile();
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

WriteAudioThread::WriteAudioThread(IAIUIAgent* agent, const string& audioPath, bool repeat):
mAgent(agent), mAudioPath(audioPath), mRepeat(repeat), mRun(true), mFileHelper(NULL)
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
		char log[2048];
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
						
					} break;

				case AIUIConstant::STATE_READY:
					{
					} break;

				case AIUIConstant::STATE_WORKING:
					{
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
				if (sub == "nlp")
				{
					Json::Value empty;
					Json::Value contentId = content.get("cnt_id", empty);

					if (contentId.empty())
					{
						sprintf(log,"onEvent Content Id is empty");
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
						break;
					}

					string cnt_id = contentId.asString();
					Buffer* buffer = event.getData()->getBinary(cnt_id.c_str());
					string resultStr;

					if (NULL != buffer)
					{
						resultStr = string((char*)buffer->data());

						sprintf(log,"onEvent %s:",resultStr.c_str());
						m_WriteLog.WriteLog(Log::DEBUG_INFO,log);
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

AIUIClient::AIUIClient() : agent(NULL), writeThread(NULL),m_WriteLog(LOG_PATH,LOG_NAME_AIUI_CLIENT)
{

}

AIUIClient::~AIUIClient() 
{
	if (agent) {
		agent->destroy();
		agent = NULL;
	}
}

void AIUIClient::createAgent()
{
	ISpeechUtility::createSingleInstance("", "",
		"appid=595ce74c");

	string paramStr = FileUtil::readFileAsString(CFG_FILE_PATH);
	agent = IAIUIAgent::createAgent(paramStr.c_str(), this);
}

void AIUIClient::wakeup()
{
	if (NULL != agent)
	{
		IAIUIMessage * wakeupMsg = IAIUIMessage::create(AIUIConstant::CMD_WAKEUP);
		agent->sendMessage(wakeupMsg);
		wakeupMsg->destroy();
	}
}

void AIUIClient::start()
{
	if (NULL != agent)
	{
		IAIUIMessage * startMsg = IAIUIMessage::create (AIUIConstant::CMD_START);
		agent->sendMessage(startMsg);
		startMsg->destroy();
	}
}

void AIUIClient::stop()
{
	if (NULL != agent)
	{
		IAIUIMessage *stopMsg = IAIUIMessage::create (AIUIConstant::CMD_STOP);
		agent->sendMessage(stopMsg);
		stopMsg->destroy();
	}
}


void AIUIClient::write(bool repeat)
{
	if (agent == NULL)
		return;

	if (writeThread == NULL) {
		writeThread = new WriteAudioThread(agent, CLIENT_AUDIO_PATH,  repeat);
		writeThread->run();
	}	
}



void AIUIClient::stopWriteThread()
{
	if (writeThread) {
		writeThread->stopRun();
		delete writeThread;
		writeThread = NULL;
	}
}

void AIUIClient::reset()
{
	if (NULL != agent)
	{
		IAIUIMessage * resetMsg = IAIUIMessage::create(AIUIConstant::CMD_RESET);
		agent->sendMessage(resetMsg);
		resetMsg->destroy();
	}
}
void AIUIClient::writeText()
{
	if (NULL != agent)
	{
		string text = "hello";
		// textData内存会在Message在内部处理完后自动release掉
		Buffer* textData = Buffer::alloc(text.length());
		text.copy((char*) textData->data(), text.length());

		IAIUIMessage * writeMsg = IAIUIMessage::create(AIUIConstant::CMD_WRITE,
			0,0, "data_type=text", textData);	

		agent->sendMessage(writeMsg);
		writeMsg->destroy();		
	}
}

void AIUIClient::destory()
{
	stopWriteThread();

	if (NULL != agent)
	{
		agent->destroy();
		agent = NULL;
	}
}

void AIUIClient::readCmd()
{
	cout << "input cmd:" << endl;

	string cmd;
	while (true)
	{
		cin >> cmd;

		if (cmd == "c")
		{
			cout << "createAgent" << endl;
			createAgent();
		} else if (cmd == "w") {
			cout << "wakeup" << endl;
			wakeup();
		} else if (cmd == "s") {
			cout << "start" << endl;
			start();
		} else if (cmd == "st") {
			cout << "stop" << endl;
			stop();
		} else if (cmd == "d") {
			cout << "destroy" << endl;
			destory();
		} else if (cmd == "r") {
			cout << "reset" << endl;
			reset();
		} else if (cmd == "e") {
			cout << "exit" << endl;
			break;
		} else if (cmd == "wr") {
			cout << "write" << endl;
			write(false);
		} else if (cmd == "wrr") {
			cout << "write repeatly" << endl;
			write(true);
		} else if (cmd == "swrt") {
			cout << "stopWriteThread" << endl;
			stopWriteThread();
		} else if (cmd == "wrt") {
			cout << "writeText" << endl;
			writeText();
		} else if (cmd == "q") {
			destory();
			break;
		} else {
			cout << "invalid cmd, input again." << endl;
		}
	}
}
void AIUIClient::test()
{
	//		AIUISetting::setSaveDataLog(true);
	AIUISetting::setAIUIDir(CLIENT_ROOT_DIR);
	AIUISetting::initLogger(LOG_DIR);

	readCmd();
}