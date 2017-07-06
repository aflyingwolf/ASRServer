// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__90F38EB7_A5E1_454D_94C0_6A699EA58051__INCLUDED_)
#define AFX_THREAD_H__90F38EB7_A5E1_454D_94C0_6A699EA58051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comLib/comdefine.h" 

namespace comLib
{
class Thread  
{
public:
	struct ThreadParam
	{
		Thread* pBaseThread;
		void*       lpParam;
	}m_sThreadParam;

	Thread();
	virtual ~Thread();
	int GetThreadId();
	bool ThreadHasEnd();
	virtual bool Stop();
	bool Start(void* lpParam = 0 );
	static void* ThreadFunc(void* lpParam);
	static void msleep(int milliseconds);
protected:
	bool   m_bRunning;
	bool   m_bHasEnd;//Thread Terminate
	int		m_ThreadId;
	bool IsExit();
	virtual void run(void* lpParam) = 0;
};


inline Thread::Thread():m_bRunning(false),m_bHasEnd(false),m_ThreadId(0)
{
}
inline Thread::~Thread()
{
}
inline bool Thread::Stop()
{
	m_bRunning = false;
	return true;
}

inline bool Thread::IsExit()
{
	return ( m_bRunning != true  );
}

inline bool Thread::ThreadHasEnd()
{
	return m_bHasEnd;
}

inline int Thread::GetThreadId()
{
	return m_ThreadId;
}
inline void* Thread::ThreadFunc(void* lpParam)
{
	struct Thread::ThreadParam* pThreadParam = (struct Thread::ThreadParam*)lpParam;
	Thread* pThread = pThreadParam->pBaseThread;
	pThread->m_bRunning = true;
	pThread->run(pThreadParam->lpParam);
	pThread->m_bRunning = false;
	pThread->m_bHasEnd = true;
	return 0;
}


////////////////////////////////////////////////////////////////////////////
#ifdef WIN32

inline bool Thread::Start(void* lpParam)
{
	m_sThreadParam.pBaseThread = this;
	m_sThreadParam.lpParam = lpParam;
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL, 0, 
			(LPTHREAD_START_ROUTINE)ThreadFunc, &m_sThreadParam, 0 , &ThreadID);
	m_ThreadId = (int)ThreadID;
	CloseHandle(hThread);
	return true;
}

inline void Thread::msleep(int milliseconds)
{
	Sleep(milliseconds);
}

////////////////////////////////////////////////////////////////////////////
#else //linux
////////////////////////////////////////////////////////////////////////////
inline bool Thread::Start(void* lpParam)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
#ifdef _AIX
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setstacksize(&attr, 1024*1024);	//the default stack size is only 96KB!
#endif
	m_sThreadParam.pBaseThread = this;
	m_sThreadParam.lpParam = lpParam;
	pthread_t ThreadID;
	//BOOL bReturn = SetEvent(m_hRunEvent);
	pthread_create(&ThreadID, &attr,
			ThreadFunc, &m_sThreadParam );
	pthread_detach(ThreadID);//分离线程

	pthread_attr_destroy(&attr);

	m_ThreadId = (int)ThreadID;
	return true;
}

inline void Thread::msleep(int milliseconds)
{
#ifndef _AIX
    struct timeval tm;    
    tm.tv_sec = 0; 
    tm.tv_usec = milliseconds * 1000; //延时毫秒 
    select(0,NULL,NULL,NULL,&tm);
#else
	usleep(milliseconds*1000);
#endif
}

#endif  // WIN32 or linux
////////////////////////////////////////////////////////////////////////////

} //namespace comLib

#endif // !defined(AFX_THREAD_H__90F38EB7_A5E1_454D_94C0_6A699EA58051__INCLUDED_)
