
// TestOneDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TestOne.h"
#include "TestOneDlg.h"
#include "afxdialogex.h"
#include <string>
#include<iostream>

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//用于双线程通信的全局变量
static bool flag = true;
//线程句柄
HANDLE ThreadHnd;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestOneDlg 对话框



CTestOneDlg::CTestOneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTONE_DIALOG, pParent)
	, readPath(_T(""))
	, writePath(_T(""))
	, Worldsize(0)
	, numSheeps(0)
	, numWolfs(0)
	, refreshThread(NULL)
	, nowSize(0)
	, nowSheeps(0)
	, nowWolfs(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestOneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, readPath);
	DDX_Text(pDX, IDC_EDIT3, writePath);
	DDX_Text(pDX, IDC_EDIT1, Worldsize);
	DDV_MinMaxLong(pDX, Worldsize, 0, 1000000000);
	DDX_Text(pDX, IDC_EDIT4, numSheeps);
	DDV_MinMaxLong(pDX, numSheeps, 0, 1000000000);
	DDX_Text(pDX, IDC_EDIT5, numWolfs);
	DDV_MinMaxLong(pDX, numWolfs, 0, 1000000000);
	DDX_Control(pDX, IDC_EDIT1, sizeWorld);
	DDX_Text(pDX, IDC_EDIT6, nowSize);
	DDV_MinMaxLong(pDX, nowSize, 0, LONG_MAX);
	DDX_Text(pDX, IDC_EDIT7, nowSheeps);
	DDV_MinMaxLong(pDX, nowSheeps, 0, LONG_MAX);
	DDX_Text(pDX, IDC_EDIT8, nowWolfs);
	DDV_MinMaxLong(pDX, nowWolfs, 0, LONG_MAX);
}

BEGIN_MESSAGE_MAP(CTestOneDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestOneDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestOneDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestOneDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestOneDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestOneDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestOneDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CTestOneDlg 消息处理程序

BOOL CTestOneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestOneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestOneDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestOneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}




void CTestOneDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (refreshThread != NULL)
	{
		flag = false;
		if (WAIT_OBJECT_0 == WaitForSingleObject(ThreadHnd, 1500))
		{
			flag = true;
			refreshThread = NULL;
		}
		else
		{
			AfxMessageBox(_T("执行失败！"));
			refreshThread = NULL;
			flag = true;
		}
	}
	UpdateData(TRUE);
	if (Worldsize < 0 || numSheeps < 0 || numWolfs < 0)
	{
		return;
	}
	if (Worldsize * Worldsize < numSheeps + numWolfs) 
	{
		AfxMessageBox(_T("世界参数有误，请重新输入！"));
		return;
	}
	else if (theApp.w == NULL && Worldsize == 0 && 
			numSheeps == 0 && numWolfs == 0)
	{
		theApp.w = new World();

		AfxMessageBox(_T("创建世界"));

		DisplayWorld();
	}
	else if (theApp.w != NULL && Worldsize == 0 && 
			numSheeps == 0 && numWolfs == 0)
	{
		World* tmp;
		tmp = theApp.w;
		theApp.w = NULL;
		delete tmp;
		theApp.w = new World();

		AfxMessageBox(_T("重新创建世界"));


		DisplayWorld();
	}
	else if (theApp.w == NULL)
	{
		theApp.w = new World(Worldsize + 2, numSheeps, numWolfs);
		
		AfxMessageBox(_T("创建世界"));
		
		DisplayWorld();
	}
	else
	{
		World* tmp;
		tmp = theApp.w;
		theApp.w = NULL;
		delete tmp;
		theApp.w = new World(Worldsize + 2, numSheeps, numWolfs);
		
		AfxMessageBox(_T("重新创建世界"));


		DisplayWorld();
	}
}

void CTestOneDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(refreshThread != NULL)
	{
	flag = false;
	if (WAIT_OBJECT_0 == WaitForSingleObject(ThreadHnd, 1500))
	{
		flag = true;
		refreshThread = NULL;
	}
	else
	{
		AfxMessageBox(_T("执行失败！"));
		refreshThread = NULL;
		flag = true;
	}
	}
	if (theApp.w == NULL)
	{
		AfxMessageBox(_T("请创建世界"));
	}
	else
	{
		theApp.w->refreshWorld();
		/*string s = to_string(theApp.w->getSheeps());;
		CA2T szr(s.c_str());

		AfxMessageBox(LPCTSTR(szr));*/


		DisplayWorld();
	}
}

void CTestOneDlg::DisplayWorld()
{
	CDC* pDC = GetDC();
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(&theApp.bitmap1);
	BITMAP bm;
	theApp.bitmap1.GetBitmap(&bm);
	CRect rect;
	GetClientRect(&rect);

	
	int aj = 170;//用来调整显示位置的参数

	rect.OffsetRect(aj, aj);
	int W = rect.Width(), H = rect.Height();
	int w = bm.bmWidth, h = bm.bmHeight;
	InvalidateRect(rect);
	UpdateWindow();
	//int i, j;
	//int grid[3][3];
	//贴羊的位图
	int size = theApp.w->getMapsize();


	
	for (int i = 0; i < size * size; i++)
	{
		int x = i % size;
		int y = i / size;

		if (theApp.w->getMappoint(x, y) != NULL)
		{
			if (theApp.w->getMappoint(x, y)->getLabel() == 2)
			{
				dc.SelectObject(&theApp.bitmap1);
				pDC->BitBlt(x * w + aj, y * h + aj, w + aj, h + aj, &dc, -30, -30, SRCCOPY);
				continue;
			}
			else if (theApp.w->getMappoint(x, y)->getLabel() == 1)
			{
				dc.SelectObject(&theApp.bitmap2);
				pDC->BitBlt(x * w + aj, y * h + aj, w + aj, h + aj, &dc, -30, -30, SRCCOPY);
				continue;
			}
			else if(theApp.w->getMappoint(x, y)->getLabel() == 3)
			{
				dc.SelectObject(&theApp.bitmap3);
				pDC->BitBlt(x * w + aj, y * h + aj, w + aj, h + aj, &dc, -30, -30, SRCCOPY);
				continue;
			}
		}
	}

	nowSize = theApp.w->getMapsize() - 2;
	nowSheeps = theApp.w->getSheeps();
	nowWolfs = theApp.w->getWolfs();
	UpdateData(false);
}



void CTestOneDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//读取存档
	UpdateData(true);
	/*const char* ch = (LPSTR)(LPCTSTR)readPath;
	World* tmp;
	tmp = theApp.w;
	theApp.w = NULL;
	delete tmp;
	theApp.w = new World(ch);*/
	loadFile();
}


void CTestOneDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.w != NULL)	
	{
		//保存存档
		UpdateData(true);
		/*const char* ch = (LPSTR)(LPCTSTR)writePath;
		theApp.w->saveWorld(ch);*/
		writeFile();
	}
	else
	{
		AfxMessageBox(_T("还未创建或加载世界！"));
	}
}

bool CTestOneDlg::writeFile()
{
	if (writePath.GetLength() == 0)
	{
		AfxMessageBox(_T("请输入写入路径"));
		return false;
	}
	if (refreshThread != NULL)
	{
		flag = false;
		if (WAIT_OBJECT_0 == WaitForSingleObject(ThreadHnd, 1500))
		{
			flag = true;
			refreshThread = NULL;
		}
		else
		{
			AfxMessageBox(_T("执行失败！"));
			refreshThread = NULL;
			flag = true;
		}
	}
	if (writePath.Right(4) != ".was")
	{
		writePath = writePath + _T(".was");
	}
	CFile outFile;
	if (outFile.Open(writePath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))
	{
		long size = theApp.w->getMapsize();
		long numSheeps = theApp.w->getSheeps();
		long numWolfs = theApp.w->getWolfs();

		outFile.Write((char*)&size, sizeof(long));
		outFile.Write((char*)&numSheeps, sizeof(long));
		outFile.Write((char*)&numWolfs, sizeof(long));

		list<Sheep>::iterator it1;
		for (it1 = theApp.w->sheeps.begin(); it1 != theApp.w->sheeps.end(); it1++)
		{
			Sheep& sheep = *it1;
			outFile.Write((char*)&sheep, sizeof(Sheep));
		}
		list<Wolf>::iterator it2;
		for (it2 = theApp.w->wolfs.begin(); it2 != theApp.w->wolfs.end(); it2++)
		{
			Wolf& wolf = *it2;
			outFile.Write((char*)&wolf, sizeof(Wolf));
		}
		AfxMessageBox(_T("已保存：") + writePath);
		return true;
	}
	else
	{
		AfxMessageBox(_T("保存失败！"));
		return false;
	}
	outFile.Close();
}

bool CTestOneDlg::loadFile()
{
	if (readPath.GetLength() == 0)
	{
		AfxMessageBox(_T("请输入读取路径"));
		return false;
	}
	if (refreshThread != NULL)
	{
		flag = false;
		if (WAIT_OBJECT_0 == WaitForSingleObject(ThreadHnd, 1500))
		{
			flag = true;
			refreshThread = NULL;
		}
		else
		{
			AfxMessageBox(_T("执行失败！"));
			refreshThread = NULL;
			flag = true;
		}
	}
	World* tmp;
	tmp = theApp.w;
	theApp.w = NULL;
	delete tmp;
	theApp.w = new World;
	
	CFile inFile;
	long size;
	long numSheeps;
	long numWolfs;

	if (inFile.Open(readPath, CFile::typeBinary | CFile::modeRead))
	{
		inFile.Read((char*)&size, sizeof(size));
		inFile.Read((char*)&numSheeps, sizeof(numSheeps));
		inFile.Read((char*)&numWolfs, sizeof(numWolfs));

		theApp.w->sheeps.clear();
		theApp.w->wolfs.clear();
		theApp.w->mapmodel.assign(size, vector<Organism*>(size, NULL));

		for (int i = 0; i < numSheeps; i++)
		{
			Sheep sheep;
			inFile.Read((char*)&sheep, sizeof(sheep));
			theApp.w->sheeps.push_back(sheep);
			theApp.w->mapmodel[sheep.getLocation()[0]][sheep.getLocation()[1]] = &theApp.w->sheeps.back();
		}
		for (int i = 0; i < numWolfs; i++)
		{
			Wolf wolf;
			inFile.Read((char*)&wolf, sizeof(wolf));
			theApp.w->wolfs.push_back(wolf);
			theApp.w->mapmodel[wolf.getLocation()[0]][wolf.getLocation()[1]] = &theApp.w->wolfs.back();
		}
		//设置树作为限制
		Tree* tmpTree = new Tree;
		theApp.w->trees.push_back(*tmpTree);
		for (int i = 0; i < size; i++)
		{
			theApp.w->mapmodel[size - 1][i] = tmpTree;
			theApp.w->mapmodel[i][size - 1] = tmpTree;
			theApp.w->mapmodel[0][i] = tmpTree;
			theApp.w->mapmodel[i][0] = tmpTree;
		}

		AfxMessageBox(_T("加载存档；") + readPath);


		DisplayWorld();
		return true;
	}
	else
	{
		AfxMessageBox(_T("读取失败！"));
		return false;
	}
}

UINT CTestOneDlg::autoRefreshWorld(LPVOID lpParam)
{
	CDC* pDC = theApp.m_pMainWnd->GetDC();
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	dc.SelectObject(&theApp.bitmap1);
	BITMAP bm;
	theApp.bitmap1.GetBitmap(&bm);
	CRect rect;
	theApp.m_pMainWnd->GetClientRect(&rect);
	int aj = 170;//用来调整显示位置的参数
	
	rect.OffsetRect(aj,aj);

	int W = rect.Width(), H = rect.Height();
	int w = bm.bmWidth, h = bm.bmHeight;

	while (flag)
	{
		theApp.w->refreshWorld();

		theApp.m_pMainWnd->InvalidateRect(rect);
		theApp.m_pMainWnd->UpdateWindow();
		//int i, j;
		//int grid[3][3];
		//贴羊的位图
		int size = theApp.w->getMapsize();

		
		
		for (int i = 0; i < size * size; i++)
		{
			int x = i % size;
			int y = i / size;
	

			if (theApp.w->getMappoint(x, y) != NULL)
			{
				if (theApp.w->getMappoint(x, y)->getLabel() == 2)
				{
					dc.SelectObject(&theApp.bitmap1);
					pDC->BitBlt(x * w + aj, y * h + aj, w + aj, h + aj, &dc, -30, -30, SRCCOPY);
					continue;
				}
				else if (theApp.w->getMappoint(x, y)->getLabel() == 1)
				{
					dc.SelectObject(&theApp.bitmap2);
					pDC->BitBlt(x * w + aj, y * h + aj, w + aj, h + aj, &dc, -30, -30, SRCCOPY);
					continue;
				}
				else if (theApp.w->getMappoint(x, y)->getLabel() == 3)
				{
					dc.SelectObject(&theApp.bitmap3);
					pDC->BitBlt(x * w + aj, y * h + aj, w + aj, h + aj, &dc, -30, -30, SRCCOPY);
					continue;
				}
			}
		}


		Sleep(500);
	}
	theApp.m_pMainWnd->ReleaseDC(pDC);
	return 0;
}

void CTestOneDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.w == NULL)
	{
		AfxMessageBox(_T("请创建世界！"));
		return;
	}
	if (refreshThread == NULL)
	{
		refreshThread = AfxBeginThread(autoRefreshWorld, NULL);
		ThreadHnd = refreshThread->m_hThread;
	}
}


void CTestOneDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.w == NULL)
	{
		AfxMessageBox(_T("请创建世界！"));
		return;
	}
	if (refreshThread == NULL)
		AfxMessageBox(_T("需开始自动执行！"));
	else
	{
		flag = false;
		if (WAIT_OBJECT_0 == WaitForSingleObject(ThreadHnd, 1500))
		{
			flag = true;
			refreshThread = NULL;

			nowSize = theApp.w->getMapsize();
			nowSheeps = theApp.w->getSheeps();
			nowWolfs = theApp.w->getWolfs();
			UpdateData(false);
		}
		else
		{
			AfxMessageBox(_T("执行失败！"));
			refreshThread = NULL;
			flag = true;
		}
	}
}
