#pragma once

class ASRThreadData
{
public:
	ASRThreadData(void);
	~ASRThreadData(void);
public:
	unsigned int ThreadId;
	HANDLE ThreadHandle; 
	//线程是否关闭
	bool bExit;
	void * pDlg;
};

