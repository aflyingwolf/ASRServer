#pragma once

class ASRThreadData
{
public:
	ASRThreadData(void);
	~ASRThreadData(void);
public:
	unsigned int ThreadId;
	HANDLE ThreadHandle; 
	//�߳��Ƿ�ر�
	bool bExit;
	void * pDlg;
};

