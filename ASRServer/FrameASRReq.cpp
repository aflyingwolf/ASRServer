#include "stdafx.h"
#include "FrameASRReq.h"
#include "FrameConst.h"
#include "stringgenc.h"
#include "Help.h"
#include "FrameNull.h"

FrameASRReq::FrameASRReq(string strHead,string strBody)
	:Frame(strHead,strBody)
{
}
FrameASRReq::FrameASRReq(void)
{
}


FrameASRReq::~FrameASRReq(void)
{
}
string FrameASRReq::ProcData()
{
	string strRet("");
	if(head.compare(FrameConst::ASR_REQ)==0)
	{
		vector<string> strList;
		split(strList,body," ",split_e::no_empties);
		if(strList.size()==5)  //����5����Ԫ��
		{
			this->taskId=strList[0];
			this->caller=strList[1];
			this->called=strList[2];
			this->type=atoi(strList[3].c_str());
			this->content=strList[4];
			strRet=FrameNull().GetFrameString();
			return strRet;
		}
		else
		{
			exception ex("��Ϣ���ֶ���Ŀ����ȷ");
			throw &ex;
		}
	}
	else
	{
		exception ex("���ݴ���");
		throw &ex;
	}
}
