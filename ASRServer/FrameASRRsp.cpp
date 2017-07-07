#include "stdafx.h"
#include "FrameASRRsp.h"
#include "stringgenc.h"
#include "FrameConst.h"

FrameASRRsp::FrameASRRsp(void)
{
	head=FrameConst::ASR_RSP;
}


FrameASRRsp::~FrameASRRsp(void)
{
}
string FrameASRRsp::GetFrameString()
{
	body=this->taskId+" "+this->caller+" "+this->called+" "+int2str(this->ret)+" "+this->fileName;
	return Frame::GetFrameString();
}