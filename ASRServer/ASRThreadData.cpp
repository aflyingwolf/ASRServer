#include "stdafx.h"
#include "ASRThreadData.h"


ASRThreadData::ASRThreadData(void)
{
	this->ThreadId=0;
	ThreadHandle=NULL;
	bExit=false;
}


ASRThreadData::~ASRThreadData(void)
{
}
