// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ChildView.h"
#include "NRSplitter.h"
#include "NRListCtrl.h"
#include "DiscSelectDlg.h"
#include "ToolBarDlg.h"
#include "NRTabNetCtrl.h"
#include "CmdLineDlg.h"
#include <map>
#include <vector>
using namespace std;


typedef map<char, PATH_DISC> CArrPathDrives;

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CReBar			m_wndReBar;
	CToolBarDlg		m_wndToolBar;
	CDiscSelectDlg  m_wndDlgBar;
	CCmdLineDlg		m_wndDlgCmdLine;
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CNRTabNetCtrl	m_tabCtrlNet;	
	// ����� ����������� ����
	CNRSplitter	m_wndSplitter; 
	// ������ ������, ������� ��������� � ������ �������
	// �� ���������������� ������
	CNRListCtrl	m_ctrlListUserFiles;
	// ������������ ������ �� ��������� ������
	afx_msg LRESULT OnSwichLocalDisc(WPARAM wp, LPARAM lp);
	// ������� � ������ �������
	afx_msg LRESULT OnChangeCatalog(WPARAM wp, LPARAM lp);
	// ���������� ������
	afx_msg LRESULT OnSetListCursor(WPARAM wp, LPARAM lp);
	CDiscSelectDlg *GetWndBar()
	{
		return &m_wndDlgBar;
	}
	CImageList *GetImageSysList(){return &m_imageListSys;}
	// ������ ���������� �����
	BOOL ExecuteFile(CString strPath, CString sz_dir, CString sz_params);
	// ������������ ������� ����� ������ �� <Tab>
	bool	m_bCursorWindow;
private:
        CDisableWOW64FSRedirection *p_fsguard; // for x64
	
	// ��������� ������ ����� ��� ������ � ������
	CImageList	m_imageListSys;
	char m_chCurrentDisc;
	// ������� ���� �� ���� ��������� ������ � �������
	CArrPathDrives m_arrPathOfDrivesLocal;
	// �������� ������ ������ � ��������� �������
	void UpdateLocalSide();
	// ������� ������� � ������ ������ �� ��������� �������
	void LoadDataDrivesLocal();
	// ����� ��� ����� �������� ������ 
	CFont  m_fontTab;
	// ���������������� ������ ��� ����������� ������ �� 
	// ��������� ������
	void InitListLocalSystem();
	// ��������� ����� ������ � ������ ������
	// szPathroot - ���� � �������� ��������
	BOOL LoadNewDataFile(CString szPathroot);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnCreatefile();
	afx_msg void OnBnClickedBtnCreatedir();
	afx_msg void OnBnClickedBtnDeletefile();
	afx_msg void OnBnClickedBtnCopy();
	afx_msg void OnBnClickedBtnMove();
	afx_msg void OnBnClickedBtnPreview();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnRefresh();
	virtual void ActivateFrame(int nCmdShow = -1);
	afx_msg void OnAccelTab();
	afx_msg void OnOpenListDrivesLeft();
	afx_msg void OnOpenListDrivesRight();
	afx_msg void OnUpdateBtnCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnCreatedir(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnCreatefile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnDeletefile(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnEdit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnMove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBtnPreview(CCmdUI *pCmdUI);
	// �������� ���� � ��������� ������
	afx_msg LRESULT OnUpdateCommandRemoute(WPARAM wp, LPARAM lp);
	// ������ ������� �� ��������� ������
	afx_msg LRESULT OnCommandLineEnter(WPARAM wp, LPARAM lp);
	// ������� �������������� �����
	afx_msg LRESULT OnCommandRenameFile(WPARAM wp, LPARAM lp);
	// ������� ����
	CString m_sz_current_path;
	afx_msg void OnUpdateFiles();
	afx_msg void OnEnterCommand();
	afx_msg void OnUpdateEnterCommand(CCmdUI *pCmdUI);
	afx_msg void OnAccelSelAll();
};


