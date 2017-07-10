
// ASRServerDlg.h : ͷ�ļ�
//

#pragma once

#include "Config.h"
#include "BaseSocket.h"
#include "Log.h"
#include "ClientList.h"
#include "FrameASRReq.h"
#include "FrameASRRsp.h"
#include <list>
#include "afxcmn.h"
#include "afxwin.h"
#include "AIUIClient.h"
#include "ASRManager.h"
#include "ClientASRDataReq.h"
#include "SemanticResultListener.h"
// CASRServerDlg �Ի���
class CASRServerDlg : public CDialog,ILog,ISemanticResultListener
{
// ����
public:
	CASRServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ASRSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//��������
	Config * pConfig;
	//�����SOCKET
	CBaseSocket ServerSocket;
	//��־��
	Log m_WriteLog;
	//�ͻ����б�
	ClientList clientList;
	ASRManager ASRManager;
public:
	//����������
	BOOL OnListen();
	//ASRServer Socket ��Ϣ����
	LONG OnASRServerSocketMsg(UINT wParam, long lParam);
	//�ͻ�������
	void Accept();
	//�ͻ��˶Ͽ�����
	void ClientDisConnect(SOCKET clientSocket);
	//�ر�SOCKET�����SOCKET��Դ
	void ClientClose(Client * pClient);
	//���յ�����
	void ReadData(SOCKET  clientSocket);
	//������յ�������
	void ProcData(Client * pClient,char * szBuffer);
	//��ͻ��˷�������
	void SendData(SOCKET clientSocket,const char * szBuffer,int nSize);
	//�ָ�����
	void SplitData(int m,char* pmsg,CString ip);
	//д��־���ڽ�����ʾ��־
	void Log(Log::LogLevel level,CString log);
	virtual void WriteLog(Log::LogLevel level,string log);
	void ProcASRReq(ClientASRDataReq clientReq);
	virtual void OnOK();
	void onEvent(SemanticResultEvent * pEvent);
	void SendFrameASRRsp(Client * pClient,FrameASRRsp rsp);
	int CreateVoxPath(const char* phone,const char * filename,const char * code, char* dpathname);
public:
	CTreeCtrl mTreeClient;
	CString listenPort;
	CListBox mListMessage;
	CButton bt_Exit;
	CString m_Message;
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonDisclient();
	afx_msg void OnLbnDblclkListMessage();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
