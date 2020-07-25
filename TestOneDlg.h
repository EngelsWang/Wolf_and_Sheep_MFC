
// TestOneDlg.h: 头文件
//

#pragma once


// CTestOneDlg 对话框
class CTestOneDlg : public CDialogEx
{
// 构造
public:
	CTestOneDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTONE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	
	afx_msg void DisplayWorld();

	// 将变量置为公共成员以加快访问速度
	// 读取路径
	CString readPath;
	// 保存路径
	CString writePath;
	// 世界大小
	long Worldsize;
	long numSheeps;
	long numWolfs;
	// 世界大小
	CEdit sizeWorld;
	// 保存文件
	bool writeFile();
	bool loadFile();

	//多线程函数
	static UINT autoRefreshWorld(LPVOID lpParam);


	//刷新世界线程指针
	CWinThread* refreshThread;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	// 当前世界大小
	long nowSize;
	// 当前羊数目
	long nowSheeps;
	// 当前狼数目
	long nowWolfs;
};
