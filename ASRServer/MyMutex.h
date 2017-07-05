// MyMutex.h: interface for the MyMutex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMUTEX_H__ACC17709_EC4F_4282_A2A8_140EDAD16312__INCLUDED_)
#define AFX_MYMUTEX_H__ACC17709_EC4F_4282_A2A8_140EDAD16312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
class MyMutex  
{
public:
	MyMutex();
	virtual ~MyMutex();
protected:
	CMutex _Mutex;
	long _lockTimes;
public:
	void Lock();
	void Unlock();
	void noop();

};

#endif // !defined(AFX_MYMUTEX_H__ACC17709_EC4F_4282_A2A8_140EDAD16312__INCLUDED_)
