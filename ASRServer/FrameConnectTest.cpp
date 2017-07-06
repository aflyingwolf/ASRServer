#include "stdafx.h"
#include "FrameConnectTest.h"
#include "FrameConst.h"

FrameConnectTest::FrameConnectTest(string strHead,string strBody)
	:Frame(strHead,strBody)
{
}
FrameConnectTest::~FrameConnectTest(void)
{
}
string FrameConnectTest::ProcData()
{
	if(this->head.compare(FrameConst::CONNECT_TEST_REQ)==0)
	{
		Frame rsp(FrameConst::CONNECT_TEST_RSP,body);
		return rsp.GetFrameString();
	}
	else
	{
		exception ex("Êý¾Ý´íÎó");
		throw &ex;
	}
}