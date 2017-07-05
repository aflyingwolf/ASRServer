
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
#include "MC_Client.h"
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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


void CASRServerDlg::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CASRServerDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CASRServerDlg::OnBnClickedButtonDisclient()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CASRServerDlg::OnLbnDblclkListMessage()
{
	// TODO: 在此添加控件通知处理程序代码
}
