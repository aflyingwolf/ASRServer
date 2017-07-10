
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
#include <direct.h>
#include <process.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CASRServerDlg 对话框
CASRServerDlg::CASRServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CASRServerDlg::IDD, pParent)
	, listenPort(_T(""))
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
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//读取配置
	if(!Config::ReadConfig(CONFIG_FILE,pConfig))
	{
		AfxMessageBox("读取配置文件失败，请检查配置文件！");
		return TRUE;
	}

	this->m_WriteLog.init(pConfig->LogPath.c_str(),LOG_NAME_PREV);
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
	if((nASRRet=ASRManager::ASRBeginInitialize(&(this->m_WriteLog)))!=0)
	{
		AfxMessageBox("ASR初始化失败！");
		OnOK();
		return FALSE;
	}
	if(!ASRManager.ASRInitConnect(this->pConfig,this))//建立连接
	{
		AfxMessageBox("ASR连接失败！");
		OnOK();
		return FALSE;
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
/******************************************************************************
*  函数名   : ProcASR(ClientASRDataReq req)
*  描  述   : 处理ASR数据
*******************************************************************************/
void CASRServerDlg::ProcASRReq(ClientASRDataReq clientReq)
{
	CString log;
	FrameASRRsp rsp;
	rsp.taskId=clientReq.req.taskId;
	rsp.caller=clientReq.req.caller;
	rsp.called=clientReq.req.called;
	rsp.ret=1;  //默认失败
	rsp.fileName="0";
	CASRServerDlg * pDlg=this;
	FrameASRReq req=clientReq.req;
	try
	{
		int nASRRet=0;
		if((nASRRet=ASRManager.ASRStartConnect())!=true)//唤醒
		{
			log.Format("CASRServerDlg::ProcASR ASRStartConnect Error=%d",nASRRet);
			pDlg->Log(Log::ERROR_INFO,log);
			SendFrameASRRsp(clientReq.pClient,rsp);
			return;
		}
		log.Format("CASRServerDlg::ProcASR Data=%s",clientReq.req.body.c_str());
		pDlg->Log(Log::MESS_INFO,log);
		pDlg->req=clientReq;
		//开始异步语义识别
		if(clientReq.req.type==1)
			ASRManager.SemanticTxt(clientReq.req.content);
		else if(clientReq.req.type==2)
		{
			char szFile[MAX_PATH];
			CreateVoxPath(clientReq.req.called.c_str(),clientReq.req.content.c_str(),"", szFile);
			ASRManager.SemanticVox(szFile);
		}
	}
	catch(...)
	{
		CString log;
		log.Format("CASRServerDlg::ProcASR ERROR");
		if(pDlg!=NULL)
			pDlg->Log(Log::ERROR_INFO,log);
	}
}
void CASRServerDlg::SendFrameASRRsp(Client * pClient,FrameASRRsp rsp)
{
	CString log;
	string strRsp=rsp.GetFrameString();	
	if(pClient->pclientSocket!=NULL)
		SendData(pClient->pclientSocket->m_Socket,strRsp.c_str(),strRsp.length());
	log.Format("SendFrameASRRsp %s:%d 数据:%s",pClient->ip.c_str(),pClient->port,strRsp.c_str());
	Log(Log::MESS_INFO,log);
}
/******************************************************************************
 *  函数名  :  void onEvent()
 *  描  述  :  语义结果
 ******************************************************************************/
void CASRServerDlg::onEvent(SemanticResultEvent * pEvent)
{
	char szLog[LOG_MAX_LENGTH];
	int nASRRet;
	if(pEvent->getEventName().compare("SementicResultTextEvent")==0)
	{
		SementicResultTextEvent * pTextEvent=dynamic_cast<SementicResultTextEvent *>(pEvent);
		FrameASRReq req=pTextEvent->pListener->req.req;
		FrameASRRsp rsp;
		rsp.taskId=req.taskId;
		rsp.caller=req.caller;
		rsp.called=req.called;
		rsp.ret=1;  //默认失败
		rsp.fileName="0";
		if(pTextEvent->text.compare("")!=0)  //有内容
		{
			sprintf(szLog,"CASRServerDlg::ProcASR SemanticTxt OK rsp=%s",pTextEvent->text.c_str());
			WriteLog(Log::MESS_INFO,szLog);
			rsp.ret=0;
			rsp.fileName=pTextEvent->text.c_str();
			sprintf(szLog,"CASRServerDlg::ProcASR SemanticTxt OK req=%s",req.content.c_str());
			WriteLog(Log::MESS_INFO,szLog);
			SendFrameASRRsp(pEvent->pListener->req.pClient,rsp);
		}
		else
		{
			//文字请求是失败，语音识别可能后续还有消息
			if(req.type==1)  
			{
				rsp.ret=1;
				rsp.fileName="0";
				string strRsp=rsp.GetFrameString();
				sprintf(szLog,"CASRServerDlg::ProcASR SemanticTxt Error");
				WriteLog(Log::MESS_INFO,szLog);
				SendFrameASRRsp(pEvent->pListener->req.pClient,rsp);
			}
		}
		if((nASRRet=ASRManager.Clean())!=0)//清除缓冲区
		{
			sprintf(szLog,"CASRServerDlg::ProcASR Clean Error=%d",nASRRet);
			WriteLog(Log::MESS_INFO,szLog);
		}
	}
}
/***************************************************************************
 *  函数名  :  SetUserPath(const char* phone,const char * filename,const char * code, char* dpathname)
 *  描  述  :  根据目录配置，号码获取最终需要读取的文件名
	参  数  :  phone:号码,根据号码来生成文件路径前缀
			   filename:文件名
			   Code:业务代码，如DHB用来区别不同的业务,可以是以|分割的多个代码。该值可能是空串，兼容旧代码
			   dpathname:输出文件服务器共享目录的位置
 *  返  回  :   小于0 :  error
 *              1 :  success
 ***************************************************************************/
int CASRServerDlg::CreateVoxPath(const char* phone,const char * filename,const char * code, char* dpathname)
{
	char path1[10],path2[10],path3[10];
	char tel[32] ={0};
	char str1[MAX_PATH] = {0};
	char str2[MAX_PATH] = {0};
	char str3[MAX_PATH] = {0};
	char str4[MAX_PATH] = {0};
	CString log;
	memset(path1,'\0',10);
	memset(path2,'\0',10);
	memset(path3,'\0',10);

	try{
		sprintf(tel,"%011s",phone);	

		//建立共享盘文件存储路径主目录
		sprintf(str1,"%s",pConfig->DataPath.c_str());
		
		// 主叫号码的1-3位
		memcpy(path1,tel,3);
		// 主叫号码的4-6位
		memcpy(path2,&tel[3],4);
		// 主叫号码的7-11位
		memcpy(path3,&tel[7],4);

		//共享盘及本地文件盘符+号码前缀路径建立
		sprintf(str3,"%s%s",str1,path1);;
		sprintf(str1,"%s\\%s",str3,path2);
		sprintf(str3,"%s\\%s",str1,path3);
		
		//文件路径建立完毕，str3存储共享盘路径

		//共享盘路径，增加业务代码的文件夹
		if(strlen(code)>0)//如果业务代码存在
		{
			//如果业务代码以|分割，逐次建立业务代码的各个子目录
			vector<string> codeList;
			split(codeList,code,"|");
			for(vector<string>::iterator it=codeList.begin();it<codeList.end();it++)
			{
				string strTempCode=*it;
				sprintf(str1,"%s\\%s",str3,strTempCode.c_str());
				strcpy(str3,str1);
			}
			sprintf(dpathname,"%s\\%s.pcm",str3,filename);
		}
		else  //业务代码不存在,判断文件名是否存在，生成共享盘文件名
		{
			if((filename!=NULL)&&(strlen(filename)>0))
				sprintf(dpathname,"%s\\%s.pcm",str3,filename);
			else
				sprintf(dpathname,"%s\\%s.pcm",str3,tel);
		}
		return 1; 
	}
	catch(...){
		CString log;
		log.Format("CreateVoxPath Error:phone=%s,filename=%s,code=%s",phone,filename,code);
		Log(Log::ERROR_INFO,log);
		return -1;
	}
}
void CASRServerDlg::OnOK() 
{
	try
	{
		CString log;
		Log(Log::MESS_INFO,"程序会在清理资源后关闭,请稍候.....");
		this->clientList.Clear();
		ASRManager::ASREndUninitialize();
		//处理结束，断开连接
		int nASRRet;
		if((nASRRet=ASRManager.ASRFinishDisconnect())!=0)//断开连接ASRSERVER
		{
			log.Format("CASRServerDlg::ProcASR ASRFinishDisconnect Error=%d",nASRRet);
			Log(Log::ERROR_INFO,log);
		}
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
void CASRServerDlg::WriteLog(Log::LogLevel level,string log)
{
	try
	{
		m_WriteLog.WriteLog(level,log.c_str());
		if(mListMessage.GetCount()>200)//超过200条记录
		{
			for(int i=0;i<195;i++)
			{
				mListMessage.DeleteString(0);
			}
		}
		CString cLog(log.c_str());
		mListMessage.InsertString(mListMessage.GetCount(),cLog);
		if(Log::ERROR_INFO==level)
			MC_AlarmSend(103,log);
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
		log.Format("ProcData %s:%d 数据:%s 开始",pClient->ip.c_str(),pClient->port,szBuffer);
		Log(Log::MESS_INFO,log);
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
					ClientASRDataReq clientReq;
					clientReq.pClient=pClient;
					clientReq.req=req;
					ProcASRReq(clientReq);
				}
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
