#include "stdafx.h"
#include "Frame.h"


Frame::Frame(void)
{
}

Frame::Frame(string strHead,string strBody)
	:head(strHead),body(strBody)
{
}
Frame::~Frame(void)
{
}
string Frame::ProcData()
{
	exception ex("��Ҫ���ظú���");
	throw &ex;
}
string Frame::GetFrameString()
{
	return head+" "+body+"#";
}
