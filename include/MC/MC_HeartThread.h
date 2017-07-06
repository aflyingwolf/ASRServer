#pragma once
#ifndef _CLIENTTHREAD_H_
#define _CLIENTTHREAD_H_
#include "comLib/Thread.h"
#include "comLib/Logger.h"

class ClientThread : public comLib::Thread
{
public:
	ClientThread(int nIndex);
	virtual ~ClientThread();
	int getThreadIndex() const{return m_nIndex;}
	virtual void run(void *lpParam);
private:
	const int m_nIndex;
	comLib::Logger	m_logger;
};
#endif