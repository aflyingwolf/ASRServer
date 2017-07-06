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
		if((strList.size()==5)||(strList.size()==6))  //允许5个或6个元素，文件名允许不存在
		{
			this->chan=strList[0];
			this->chanState=strList[1];
			if(!Help::isPhone(strList[2].c_str()))
			{
				exception ex("主叫号码格式不正确");
				throw &ex;
			}
			this->caller=strList[2];
			//长度固定为1
			this->fileNum=1;
			this->ASRData=strList[4];
			if(strList.size()==5)//没有传文件名，手机号即是文件名
			{
				this->fileName=this->caller;
				this->code="";
				strRet=FrameNull().GetFrameString();
			}
			else
			{
				vector<string> strList2;
				split(strList2,strList[5],":",split_e::no_empties);
				if((strList2.size()==1)||(strList2.size()==2))//数据有效
				{
					this->fileName=strList2[0];
					if(strList2.size()==2)
						this->code=strList2[1];
					strRet=FrameNull().GetFrameString();
				}
				else
				{
					exception ex("文件名格式不正确");
					throw &ex;
				}
			}
			return strRet;
		}
		else
		{
			exception ex("消息包字段数目不正确");
			throw &ex;
		}
	}
	else
	{
		exception ex("数据错误");
		throw &ex;
	}
}
