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
		if((strList.size()==5)||(strList.size()==6))  //����5����6��Ԫ�أ��ļ�����������
		{
			this->chan=strList[0];
			this->chanState=strList[1];
			if(!Help::isPhone(strList[2].c_str()))
			{
				exception ex("���к����ʽ����ȷ");
				throw &ex;
			}
			this->caller=strList[2];
			//���ȹ̶�Ϊ1
			this->fileNum=1;
			this->ASRData=strList[4];
			if(strList.size()==5)//û�д��ļ������ֻ��ż����ļ���
			{
				this->fileName=this->caller;
				this->code="";
				strRet=FrameNull().GetFrameString();
			}
			else
			{
				vector<string> strList2;
				split(strList2,strList[5],":",split_e::no_empties);
				if((strList2.size()==1)||(strList2.size()==2))//������Ч
				{
					this->fileName=strList2[0];
					if(strList2.size()==2)
						this->code=strList2[1];
					strRet=FrameNull().GetFrameString();
				}
				else
				{
					exception ex("�ļ�����ʽ����ȷ");
					throw &ex;
				}
			}
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
