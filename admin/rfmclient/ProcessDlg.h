#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"

// CProcessDlg dialog

class CProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessDlg();
	// ���������� �������� ��������� ��������
	void SetMaxProcess(ULONGLONG nMax);
	// �������� �������� ��������
	void UpdateProgress(ULONGLONG nCurrentPos);	
	// ���������� �������� �������� �����������
	void SetMaxProcessCopy(ULONGLONG nMax);
	// �������� ��������  �����������
	void UpdateProgressCopy(ULONGLONG nCurrentPos);	
	// ���������� ����� 1
	void SetText1(CString str);
	// ���������� ����� 2
	void SetText2(CString str);
	// ���������� ��� ���� ��������
	void SetNameWnd(CString szName){m_strNameWnd = szName;}
	HWND get_hwnd_process() const {return m_hwnd_progress;}
// Dialog Data
	enum { IDD = IDD_PROCESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBreakOperation();
	// ��������� �� ���������� �������
	afx_msg LRESULT OnUpdateText(WPARAM wp, LPARAM lp);
	// ��������� �� ���������� �������� ���������
	afx_msg LRESULT OnUpdateProgress(WPARAM wp, LPARAM lp);
	// ��������� �� ���������� ������������ ������� �������� ��������
	afx_msg  LRESULT OnUpdatePosMax(WPARAM wp, LPARAM lp);
	// ��������� �� ���������� �������� �����������
	afx_msg LRESULT OnUpdateProgressCopy(WPARAM wp, LPARAM lp);
	// ��������� �� ���������� ������������ ������� �������� �����������
	afx_msg  LRESULT OnUpdatePosMaxCopy(WPARAM wp, LPARAM lp);
private:
	// ������� ������� �������� ����������
	ULONGLONG m_liSizeCurrent;
	// ������� ������� �������� �����������
	ULONGLONG m_liSizeCurrentCopy;
	// ������� �������� ���������
	ULONGLONG m_liSizeMax;
	// ������� �������� �����������
	ULONGLONG m_liSizeMaxCopy;
	// �������� ��������
	CString m_strNameWnd;
	// �������� ��������
	CProgressCtrl m_ctrlProgress;
	// �������� �����������
	CProgressCtrl m_ctrlProgressCopy;
	// ������� ������ ������
	HANDLE m_h_thread_start;
	// ������� �������� ������
	HANDLE m_h_thread_stop;
	// ����� ������ �� ���������
	HANDLE m_h_thread_was_stoped;
	// ������ ������ �������
	CRITICAL_SECTION m_cs_data_status;
	// ���� ����������� ��������
	bool *m_pb_flag_global_stop;
	HWND m_hwnd_progress;
public:
	CStatic m_ctrText1;
	CStatic m_ctrText2;
public:
	virtual BOOL OnInitDialog();
	// ���������� ������� ������ ������
	void set_event_start_thread(HANDLE h_thread_start){m_h_thread_start=h_thread_start;}
	// ���������� ������� ��� �������� ������
	void set_event_stop_thread(HANDLE h_thread_stop){m_h_thread_stop=h_thread_stop;}
	// ���������� ������� ��� ��� ��������� ���� ������� �� ���������
	void set_event_was_stoped_thread(HANDLE h_thread_was_stoped){m_h_thread_was_stoped=h_thread_was_stoped;}
	// ��������� ���� ����������� �������� ��������
	void set_flag_global_stop(bool *pb_flag_stop){m_pb_flag_global_stop = pb_flag_stop;}
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
