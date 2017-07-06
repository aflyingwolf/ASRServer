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
	body=chan+" "+chanState+" "+caller+" "+int2str(fileNum)+" "+fileName;
	return Frame::GetFrameString();
}