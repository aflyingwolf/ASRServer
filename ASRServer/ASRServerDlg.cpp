
// ASRServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ASRServer.h"
#include "ASRServerDlg.h"

#include "direct.h"
#include "stringgenc.h"
#include "Help.h"
#include "Frame.h"
#include "FrameConst.h"
#include "FrameConnectTest.h"
#include "FrameNull.h"
#include "FrameASRReq.h"
#include "ASRManager.h"
#include "FrameASRRsp.h"
#include "MC_Client.h"
#include <process.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CASRServerDlg �Ի���
CASRServerDlg::CASRServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CASRServerDlg::IDD, pParent)
	, listenPort(_T("")),m_WriteLog(LOG_PATH,LOG_NAME_PREV)
	, m_Message(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CASRServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CLIENT, mTreeClient);
	DDX_Text(pDX, IDC_EDIT_LISTEN_PORT, listenPort);
	DDX_Control(pDX, IDC_LIST_MESSAGE, mListMessage);
	DDX_Control(pDX, IDC_BUTTON_EXIT, bt_Exit);
	DDX_Text(pDX, IDC_EDIT_MESS, m_Message);
}

BEGIN_MESSAGE_MAP(CASRServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ASRSERVER_SOCKET_MSG,OnASRServerSocketMsg)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CASRServerDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CASRServerDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_DISCLIENT, &CASRServerDlg::OnBnClickedButtonDisclient)
	ON_LBN_DBLCLK(IDC_LIST_MESSAGE, &CASRServerDlg::OnLbnDblclkListMessage)
END_MESSAGE_MAP()


// CASRServerDlg ��Ϣ�������

BOOL CASRServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ȡ����
	if(!Config::ReadConfig(CONFIG_FILE,pConfig))
	{
		AfxMessageBox("��ȡ�����ļ�ʧ�ܣ����������ļ���");
		return TRUE;
	}
	MC_Init(pConfig->AppName,pConfig->HeartTimerInterval);
	Log(Log::MESS_INFO,"ASRServer ����");
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//�����SOCKET����
	if (!OnListen())
	{
		AfxMessageBox("����ʧ��");
		OnOK();
		return FALSE;
	}
	this->listenPort.Format("%d",pConfig->Port);
	UpdateData(false);

	int nASRRet=0;
	if((nASRRet=ASRManager::ASRBeginInitialize())!=0)
	{
		AfxMessageBox("ASR��ʼ��ʧ�ܣ�");
		OnOK();
		return FALSE;
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
/******************************************************************************
*  ������   : ProcASR(FrameASRReq req)
*  ��  ��   : ����ASR����
*******************************************************************************/
FrameASRRsp CASRServerDlg::ProcASRReq(FrameASRReq req)
{
	CString log;
	FrameASRRsp rsp;
	rsp.taskId=req.taskId;
	rsp.caller=req.caller;
	rsp.called=req.called;
	rsp.ret=1;  //Ĭ��ʧ��
	CASRServerDlg * pDlg=this;
	try
	{
		int nASRRet=0;
		ASRManager ASRManager;
		if(!ASRManager.ASRInitConnect(*this->pConfig))//��������
		{
			log.Format("CASRServerDlg::ProcASR ASRInitConnect Error");
			pDlg->Log(Log::ERROR_INFO,log);
			return rsp;
		}
		if((nASRRet=ASRManager.ASRStartConnect())!=true)//����
		{
			log.Format("CASRServerDlg::ProcASR ASRStartConnect Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
			return rsp;
		}
		log.Format("CASRServerDlg::ProcASR Data=%s",req.body.c_str());
		pDlg->Log(Log::MESS_INFO,log);

		//��ʼ����ʶ��
		nASRRet=ASRManager.SemanticTxt(req.content,rsp.fileName);
		if(nASRRet!=0)//ʶ��ʧ��
		{
			rsp.ret=1;
			rsp.fileName="0";
			string strRsp=rsp.GetFrameString();
			log.Format("CASRServerDlg::ProcASR SemanticTxt Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
		}
		else//�ϳɳɹ�
		{
			log.Format("CASRServerDlg::ProcASR SemanticTxt OK %s",rsp.fileName.c_str());
			pDlg->Log(Log::MESS_INFO,log);
			rsp.ret=0;
			rsp.fileName="OK";
			string strRsp=rsp.GetFrameString();
			log.Format("CASRServerDlg::ProcASR SemanticTxt OK %s",req.content.c_str());
			pDlg->Log(Log::MESS_INFO,log);
		}
		if((nASRRet=ASRManager.Clean())!=0)//���������
		{
			log.Format("CASRServerDlg::ProcASR Clean Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
		}
		//����������Ͽ�����
		if((nASRRet=ASRManager.ASRFinishDisconnect())!=0)//�Ͽ�����ASRSERVER
		{
			log.Format("CASRServerDlg::ProcASR ASRFinishDisconnect Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
		}
	}
	catch(...)
	{
		CString log;
		log.Format("CASRServerDlg::ProcASR ERROR");
		if(pDlg!=NULL)
			pDlg->Log(Log::ERROR_INFO,log);
	}
	if(pDlg!=NULL)
		pDlg->Log(Log::MESS_INFO,"CASRServerDlg::ProcASR EXIT");
	return rsp;
}
void CASRServerDlg::OnOK() 
{
	try
	{
		Log(Log::MESS_INFO,"�������������Դ��ر�,���Ժ�.....");
		this->clientList.Clear();
		ASRManager::ASREndUninitialize();
		Config::ClearConfig();
		m_WriteLog.WriteLog(Log::MESS_INFO,"ASRServer �ر�");
		MC_Stop();
		CDialog::OnOK();
	}
	catch(...)
	{
		CString log;
		log.Format("CASRServerDlg::OnOK ERROR");
		Log(Log::ERROR_INFO,log);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CASRServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CASRServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CASRServerDlg::OnListen() 
{
	// TODO: Add your control notification handler code here
	//create socket
	SOCKET nSocket = this->ServerSocket.Socket();
	if(nSocket < 0)
	{
		AfxMessageBox("OnListen():���������SOCKET ERROR");
		return FALSE;
	}
	//bind port
	int nRetuCode = ServerSocket.Bind(pConfig->Port); //must be specified clearly!
	if(nRetuCode <= 0) 
	{
		char str[256]={0};
		sprintf(str,"OnListen:tcp�˿�%d�ʧ�ܣ�����fport���",pConfig->Port);
		AfxMessageBox(str);
		return FALSE;
	}

	//Listen socket
	nRetuCode = ServerSocket.Listen(256);
	if(nRetuCode < 0) 
	{
		AfxMessageBox("OnListen:����ʧ��");
		return FALSE;
	}

	//Async socket message
	nRetuCode = ServerSocket.AsyncSelect(m_hWnd, WM_ASRSERVER_SOCKET_MSG, 
							FD_READ|FD_WRITE|FD_ACCEPT|FD_CLOSE);
	if(nRetuCode < 0) 
	{
		AfxMessageBox("OnListen:Error in AsyncSelect()");
		return FALSE;
	}
	Log(Log::MESS_INFO,"���������ɹ�");
	return TRUE;
}
void CASRServerDlg::Log(Log::LogLevel level,CString log)
{
	try
	{
		m_WriteLog.WriteLog(level,log);
		if(mListMessage.GetCount()>200)//����200����¼
		{
			for(int i=0;i<195;i++)
			{
				mListMessage.DeleteString(0);
			}
		}
		mListMessage.InsertString(mListMessage.GetCount(),log);
		if(Log::ERROR_INFO==level)
			MC_AlarmSend(103,CString2Char(log));
		//UpdateData(false);
	}
	catch(...)
	{
		;//m_WriteLog.WriteLog(Log::ERROR_INFO,"CIFlyASRServerDlg::Log ʧ��");
	}
}
void CASRServerDlg::Accept()
{
	try
	{
		SOCKADDR_IN sockaddrout;
		int RetSock = ServerSocket.Accept(&sockaddrout);
		if( RetSock < 0 ) 			
		{
			CString log;
			log.Format("socket accept error");
			Log(Log::ERROR_INFO,log); 
			return ;
		}
		char * strIP;
		strIP=inet_ntoa(sockaddrout.sin_addr);
		CClientSocket * pClientSocket=NULL;
		pClientSocket=new CClientSocket(RetSock,sockaddrout);
		if(pClientSocket==NULL)
		{
			Log(Log::ERROR_INFO,"�ͻ���SOCKET����ʧ�ܣ�");
			return ;
		}
		Client * pClient=new Client(strIP,sockaddrout.sin_port,pClientSocket);
		this->clientList.AddClient(pClient);
		//��������
		HTREEITEM hItem,hSubItem0,hSubItem1;
		CString str;
		str.Format("%s:%d",strIP,sockaddrout.sin_port);
		Log(Log::MESS_INFO,str+"����");
		hItem = this->mTreeClient.InsertItem(str,TVI_ROOT);
		str.Format("SOCKET:%d",RetSock);
		hSubItem0 = mTreeClient.InsertItem(strIP,hItem);
		hSubItem1 = mTreeClient.InsertItem(str,hItem,hSubItem0);
	}
	catch(...)
	{
		Log(Log::ERROR_INFO,"Accept Error");
	}
}
void CASRServerDlg::SendData(SOCKET clientSocket,const char * szBuffer,int nSize)
{
	try
	{
		CString log;
		log.Format("��������:Socket=%d,strBuffer=%s",clientSocket,szBuffer);
		Log(Log::MESS_INFO,log); 
		this->clientList.ClientSend(clientSocket,szBuffer,nSize);
	}
	catch(...)
	{
		Log(Log::ERROR_INFO,"SendData Error");
	}
}
void CASRServerDlg::ClientDisConnect(SOCKET clientSocket)
{
	try
	{
		Client * pClient=this->clientList.RemoveClient(clientSocket);
		if(pClient!=NULL)
		{
			//�������
			CString str;
			str.Format("%s:%d",pClient->ip.c_str(),pClient->port);
			HTREEITEM hItem = mTreeClient.GetRootItem();
			while(hItem!=NULL)
			{
				CString strText = mTreeClient.GetItemText(hItem);
				if(strText.Compare(str)==0)
				{
					mTreeClient.DeleteItem(hItem);
					break;
				}
				hItem = mTreeClient.GetNextSiblingItem(hItem);
			}
			ClientClose(pClient);
			Log(Log::MESS_INFO,str+"�Ͽ�����");
		}
	}
	catch(...)
	{
		Log(Log::MESS_INFO,"ClientDisConnect Error:"+clientSocket);
	}
}
void CASRServerDlg::ClientClose(Client * pClient)
{
	try
	{
		if(pClient!=NULL)
		{
			//SOCKET�ر�
			if(pClient->pclientSocket!=NULL)
			{
				closesocket(pClient->pclientSocket->m_Socket);
				//����ɾ��
				delete pClient->pclientSocket;
			}
			pClient->pclientSocket=NULL;
			delete pClient;
			pClient=NULL;
		}
	}
	catch(...)
	{
		Log(Log::MESS_INFO,"ClientClose Error:");
	}
}
//���յ�����
void CASRServerDlg::ReadData(SOCKET  clientSocket)
{
	try
	{
		//�������ĸ�SOCKET
		Client * pClient=clientList.FindClient(clientSocket);
		if(pClient!=NULL)
		{
			CString log;
			char Buf[BLOCK_SIZE+1];
			memset(Buf,0,sizeof(Buf));
			if(pClient->pclientSocket->Recv(Buf)<0)//��������ʧ�ܣ�����������Ͽ�
			{
				log.Format("���յ� %s:%d ����:����",pClient->ip.c_str(),pClient->port);
				Log(Log::ERROR_INFO,log);
				ClientDisConnect(clientSocket);
				return;
			}
			//�����������
			log.Format("���յ� %s:%d ����:%s",pClient->ip.c_str(),pClient->port,Buf);
			Log(Log::MESS_INFO,log);
			ProcData(pClient,Buf);
		}
	}
	catch(...)
	{
		Log(Log::ERROR_INFO,"ReadData Error:"+clientSocket);
	}
}
void CASRServerDlg::ProcData(Client * pClient,char * szBuffer)
{
	CString log;
	try
	{
		vector <string> packetList;
		split(packetList,szBuffer,"#",split_e::no_empties);
		for(vector <string>::iterator it=packetList.begin();
			it!=packetList.end();it++)
		{
			vector <string> fields;
			split(fields,*it," ",2,split_e::no_empties);
			if(fields.size()==2)
			{
				string strRsp="";
				if(fields[0]==FrameConst::CONNECT_TEST_REQ)
				{
					FrameConnectTest Req(fields[0],fields[1]);
					strRsp=Req.ProcData();
				}
				else if(fields[0]==FrameConst::ASR_REQ)//�ϳ�����
				{
					FrameASRReq req(fields[0],fields[1]);
					req.ProcData();
					FrameASRRsp rsp=ProcASRReq(req);
					strRsp=rsp.GetFrameString();	
				}
				//������ش������ݣ�����
				if(strRsp.compare("")!=0)
				{
					if(pClient->pclientSocket!=NULL)
						SendData(pClient->pclientSocket->m_Socket,strRsp.c_str(),strRsp.length());
				}
				log.Format("ProcData ��� %s:%d ����:%s",pClient->ip.c_str(),pClient->port,szBuffer);
				Log(Log::MESS_INFO,log);
			}
			else
			{
				log.Format("ProcData %s:%d ����:%s ��ʽ����",pClient->ip.c_str(),pClient->port,szBuffer);
				Log(Log::ERROR_INFO,log);
			}
		}
	}
	catch (std::exception& ex)
	{
		log.Format("ProcData %s:%d ����:%s ����%s",pClient->ip.c_str(),pClient->port,szBuffer,ex.what());
		Log(Log::ERROR_INFO,log);
	}
	catch(...)
	{
		log.Format("ProcData %s:%d ����:%s ����",pClient->ip.c_str(),pClient->port,szBuffer);
		Log(Log::ERROR_INFO,log);
	}
}

LONG CASRServerDlg::OnASRServerSocketMsg(UINT wParam, long lParam)
{
	try
	{
		if(WSAGETSELECTERROR(lParam))
		{
			//An error occured
			Log(Log::ERROR_INFO,"SOCKET����:Socket="+wParam);
			ClientDisConnect(wParam);
			return 0;
		}

		switch(WSAGETSELECTEVENT(lParam))
		{
			case FD_READ:   //��������
				ReadData(wParam);
				break;
			case FD_WRITE:
				break;
			case FD_ACCEPT://Receive client connect request			
				Accept();
				break;
			case FD_CLOSE: 
				ClientDisConnect(wParam);
				break;
			default:
				break;
		}
	}
	catch(...)
	{
		CString log;
		log.Format("OnASRServerSocketMsg Error");
		Log(Log::ERROR_INFO,log);
	}
	return 1L;
}


void CASRServerDlg::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->bt_Exit.EnableWindow(false);
	OnOK();
}

void CASRServerDlg::OnBnClickedButtonClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	try
	{
		mListMessage.ResetContent();
	}
	catch(...)
	{
		CString log;
		log.Format("OnBnClickedButtonClear Error");
		Log(Log::ERROR_INFO,log);
	}
}

void CASRServerDlg::OnBnClickedButtonDisclient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString log;
	try
	{
		HTREEITEM hItem = mTreeClient.GetSelectedItem();
		HTREEITEM hChild=mTreeClient.GetChildItem(hItem);
		if(hChild!=NULL)
		{
			CString str;
			str = mTreeClient.GetItemText(hChild);
			if(str.Find(":")==-1)  //û���ҵ����ָ�
			{
				hChild=mTreeClient.GetNextSiblingItem(hChild);
				str = mTreeClient.GetItemText(hChild);
			}
			vector<string> fieldsList;
			string strPort=CString2Char(str);
			split(fieldsList,strPort,":",split_e::no_empties);
			if(fieldsList.size()==2)
			{
				strPort=fieldsList[1];
				int Port=str2int(strPort);
				Client * pClient=this->clientList.RemoveClient(Port);
				if(pClient!=NULL)  //���ҵ��ͻ��ˣ������û�ѡ��Ĵ���
				{
					log.Format("�ֶ��Ͽ�����:%s:%d",pClient->ip.c_str(),pClient->port);
					Log(Log::ERROR_INFO,log);
					mTreeClient.DeleteItem(hItem);
					ClientClose(pClient);
				}
			}
		}
		else
		{
			::AfxMessageBox("��ѡ��ĳ�ͻ��˸�Ŀ¼");
		}
	}
	catch(...)
	{
		log.Format("OnBnClickedButtonDisclient Error");
		Log(Log::ERROR_INFO,log);
	}
}

void CASRServerDlg::OnLbnDblclkListMessage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	try
	{
		CString str;
		this->mListMessage.GetText(mListMessage.GetCurSel(),str);
		this->m_Message=str;
		UpdateData(FALSE);
	}
	catch(...)
	{
		CString log;
		log.Format("OnLbnDblclkListMessage Error");
		Log(Log::ERROR_INFO,log);
	}
}
