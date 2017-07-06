// ChannelWorker.cpp: implementation of the ChannelWorker class.
//
//////////////////////////////////////////////////////////////////////
#include "MC_HeartThread.h"
#include "MC_DllData.h"
#include "MC_HttpReq.h"
#include "MC_Tools.h"
ClientThread::ClientThread(int nIndex):m_nIndex(nIndex),m_logger("ClientHeartThread")
{

}
ClientThread::~ClientThread()
{

}
void ClientThread::run(void* lpParam)
{
	try
	{
		clock_t start,finish;
		start = clock();
		CStringW Url=CA2W(DllData::GetInstance()->GetStrUrl().c_str());
		while(true)
		{
			finish = clock();
			if (DllData::GetInstance()->GetHearTimer()>0)
			{
				if ((finish-start)/ CLOCKS_PER_SEC>=DllData::GetInstance()->GetHearTimer())
				{			
					char XmlMsg[1024]={0};
					std::string strAPPmsg="ÐÄÌøÐÅÏ¢";
					sprintf(XmlMsg,"xml=%s",CreatWarXML(DllData::GetInstance()->GetStrAppName(),0,strAPPmsg).c_str());	
					CStringW APPMsg=CA2W(XmlMsg);
					WinHttp::GetInstance()->HttpRequest(Url,APPMsg);
					start = clock();
				}
				else
				{
					Sleep(10000);
				}
			}
			else
			{

			}
		}
	}
	catch (...)
	{

	}
}
