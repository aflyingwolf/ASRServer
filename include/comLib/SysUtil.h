// SysUtil.h: interface for the SysUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSUTIL_H__413D2D24_ED1A_4D82_AA1D_29E0F0679B8E__INCLUDED_)
#define AFX_SYSUTIL_H__413D2D24_ED1A_4D82_AA1D_29E0F0679B8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
namespace comLib
{

class SysUtil  
{
public:
	static bool CreateLongDirectory(const std::string& strDirectory);
	static void SleepMillisecond(unsigned long millisecond);
	static void SleepSecond(unsigned long second);

};
} //namespace comLib

#endif // !defined(AFX_SYSUTIL_H__413D2D24_ED1A_4D82_AA1D_29E0F0679B8E__INCLUDED_)
