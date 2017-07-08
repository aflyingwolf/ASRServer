
// ASRServerDlg.cpp : 实现文件
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


// CASRServerDlg 对话框
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


// CASRServerDlg 消息处理程序

BOOL CASRServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//读取配置
	if(!Config::ReadConfig(CONFIG_FILE,pConfig))
	{
		AfxMessageBox("读取配置文件失败，请检查配置文件！");
		return TRUE;
	}
	MC_Init(pConfig->AppName,pConfig->HeartTimerInterval);
	Log(Log::MESS_INFO,"ASRServer 启动");
	// TODO: 在此添加额外的初始化代码
	//服务端SOCKET侦听
	if (!OnListen())
	{
		AfxMessageBox("侦听失败");
		OnOK();
		return FALSE;
	}
	this->listenPort.Format("%d",pConfig->Port);
	UpdateData(false);

	int nASRRet=0;
	if((nASRRet=ASRManager::ASRBeginInitialize())!=0)
	{
		AfxMessageBox("ASR初始化失败！");
		OnOK();
		return FALSE;
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
/******************************************************************************
*  函数名   : ProcASR(FrameASRReq req)
*  描  述   : 处理ASR数据
*******************************************************************************/
FrameASRRsp CASRServerDlg::ProcASRReq(FrameASRReq req)
{
	CString log;
	FrameASRRsp rsp;
	rsp.taskId=req.taskId;
	rsp.caller=req.caller;
	rsp.called=req.called;
	rsp.ret=1;  //默认失败
	CASRServerDlg * pDlg=this;
	try
	{
		int nASRRet=0;
		ASRManager ASRManager;
		if(!ASRManager.ASRInitConnect(*this->pConfig))//建立连接
		{
			log.Format("CASRServerDlg::ProcASR ASRInitConnect Error");
			pDlg->Log(Log::ERROR_INFO,log);
			return rsp;
		}
		if((nASRRet=ASRManager.ASRStartConnect())!=true)//唤醒
		{
			log.Format("CASRServerDlg::ProcASR ASRStartConnect Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
			return rsp;
		}
		log.Format("CASRServerDlg::ProcASR Data=%s",req.body.c_str());
		pDlg->Log(Log::MESS_INFO,log);

		//开始语义识别
		nASRRet=ASRManager.SemanticTxt(req.content,rsp.fileName);
		if(nASRRet!=0)//识别失败
		{
			rsp.ret=1;
			rsp.fileName="0";
			string strRsp=rsp.GetFrameString();
			log.Format("CASRServerDlg::ProcASR SemanticTxt Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
		}
		else//合成成功
		{
			log.Format("CASRServerDlg::ProcASR SemanticTxt OK %s",rsp.fileName.c_str());
			pDlg->Log(Log::MESS_INFO,log);
			rsp.ret=0;
			rsp.fileName="OK";
			string strRsp=rsp.GetFrameString();
			log.Format("CASRServerDlg::ProcASR SemanticTxt OK %s",req.content.c_str());
			pDlg->Log(Log::MESS_INFO,log);
		}
		if((nASRRet=ASRManager.Clean())!=0)//清除缓冲区
		{
			log.Format("CASRServerDlg::ProcASR Clean Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
		}
		//处理结束，断开连接
		if((nASRRet=ASRManager.ASRFinishDisconnect())!=0)//断开连接ASRSERVER
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
		Log(Log::MESS_INFO,"程序会在清理资源后关闭,请稍候.....");
		this->clientList.Clear();
		ASRManager::ASREndUninitialize();
		Config::ClearConfig();
		m_WriteLog.WriteLog(Log::MESS_INFO,"ASRServer 关闭");
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CASRServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
		AfxMessageBox("OnListen():创建服务端SOCKET ERROR");
		return FALSE;
	}
	//bind port
	int nRetuCode = ServerSocket.Bind(pConfig->Port); //must be specified clearly!
	if(nRetuCode <= 0) 
	{
		char str[256]={0};
		sprintf(str,"OnListen:tcp端口%d邦定失败，请用fport检查",pConfig->Port);
		AfxMessageBox(str);
		return FALSE;
	}

	//Listen socket
	nRetuCode = ServerSocket.Listen(256);
	if(nRetuCode < 0) 
	{
		AfxMessageBox("OnListen:侦听失败");
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
	Log(Log::MESS_INFO,"设置侦听成功");
	return TRUE;
}
void CASRServerDlg::Log(Log::LogLevel level,CString log)
{
	try
	{
		m_WriteLog.WriteLog(level,log);
		if(mListMessage.GetCount()>200)//超过200条记录
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
		;//m_WriteLog.WriteLog(Log::ERROR_INFO,"CIFlyASRServerDlg::Log 失败");
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
			Log(Log::ERROR_INFO,"客户端SOCKET接受失败！");
			return ;
		}
		Client * pClient=new Client(strIP,sockaddrout.sin_port,pClientSocket);
		this->clientList.AddClient(pClient);
		//界面增加
		HTREEITEM hItem,hSubItem0,hSubItem1;
		CString str;
		str.Format("%s:%d",strIP,sockaddrout.sin_port);
		Log(Log::MESS_INFO,str+"连接");
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
		log.Format("发送数据:Socket=%d,strBuffer=%s",clientSocket,szBuffer);
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
			//界面清除
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
			Log(Log::MESS_INFO,str+"断开连接");
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
			//SOCKET关闭
			if(pClient->pclientSocket!=NULL)
			{
				closesocket(pClient->pclientSocket->m_Socket);
				//对象删除
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
//接收到数据
void CASRServerDlg::ReadData(SOCKET  clientSocket)
{
	try
	{
		//查找是哪个SOCKET
		Client * pClient=clientList.FindClient(clientSocket);
		if(pClient!=NULL)
		{
			CString log;
			char Buf[BLOCK_SIZE+1];
			memset(Buf,0,sizeof(Buf));
			if(pClient->pclientSocket->Recv(Buf)<0)//接收数据失败，可能是网络断开
			{
				log.Format("接收到 %s:%d 数据:出错",pClient->ip.c_str(),pClient->port);
				Log(Log::ERROR_INFO,log);
				ClientDisConnect(clientSocket);
				return;
			}
			//接收数据完成
			log.Format("接收到 %s:%d 数据:%s",pClient->ip.c_str(),pClient->port,Buf);
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
				else if(fields[0]==FrameConst::ASR_REQ)//合成数据
				{
					FrameASRReq req(fields[0],fields[1]);
					req.ProcData();
					FrameASRRsp rsp=ProcASRReq(req);
					strRsp=rsp.GetFrameString();	
				}
				//如果返回串有内容，则发送
				if(strRsp.compare("")!=0)
				{
					if(pClient->pclientSocket!=NULL)
						SendData(pClient->pclientSocket->m_Socket,strRsp.c_str(),strRsp.length());
				}
				log.Format("ProcData 完成 %s:%d 数据:%s",pClient->ip.c_str(),pClient->port,szBuffer);
				Log(Log::MESS_INFO,log);
			}
			else
			{
				log.Format("ProcData %s:%d 数据:%s 格式错误",pClient->ip.c_str(),pClient->port,szBuffer);
				Log(Log::ERROR_INFO,log);
			}
		}
	}
	catch (std::exception& ex)
	{
		log.Format("ProcData %s:%d 数据:%s 错误%s",pClient->ip.c_str(),pClient->port,szBuffer,ex.what());
		Log(Log::ERROR_INFO,log);
	}
	catch(...)
	{
		log.Format("ProcData %s:%d 数据:%s 错误",pClient->ip.c_str(),pClient->port,szBuffer);
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
			Log(Log::ERROR_INFO,"SOCKET错误:Socket="+wParam);
			ClientDisConnect(wParam);
			return 0;
		}

		switch(WSAGETSELECTEVENT(lParam))
		{
			case FD_READ:   //接收数据
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
	// TODO: 在此添加控件通知处理程序代码
	this->bt_Exit.EnableWindow(false);
	OnOK();
}

void CASRServerDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CString log;
	try
	{
		HTREEITEM hItem = mTreeClient.GetSelectedItem();
		HTREEITEM hChild=mTreeClient.GetChildItem(hItem);
		if(hChild!=NULL)
		{
			CString str;
			str = mTreeClient.GetItemText(hChild);
			if(str.Find(":")==-1)  //没有找到：分割
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
				if(pClient!=NULL)  //查找到客户端，可能用户选择的错误
				{
					log.Format("手动断开连接:%s:%d",pClient->ip.c_str(),pClient->port);
					Log(Log::ERROR_INFO,log);
					mTreeClient.DeleteItem(hItem);
					ClientClose(pClient);
				}
			}
		}
		else
		{
			::AfxMessageBox("请选择某客户端根目录");
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
	// TODO: 在此添加控件通知处理程序代码
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
