#include "MC_DllData.h"
DllData* DllData::m_pInstance=NULL;

DllData::DllData():m_hThread(NULL)
{

}
DllData::~DllData()
{
	
}

DllData* DllData::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new DllData();
	}
	return m_pInstance;
}
