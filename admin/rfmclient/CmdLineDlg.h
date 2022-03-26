#pragma once
#include "HistoryComboBox.h"

// CCmdLineDlg dialog

class CCmdLineDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CCmdLineDlg)

public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	CCmdLineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCmdLineDlg();
	// ���������� ����
	void SetPathStatic(CString sz_path);
	// ���������� ����
	CString GetPathStatic();
	// ���������� ��� �����
	void SetFileName(CString sz_f_name);
	// ���������� ��� �����
	CString GetFileName();

// Dialog Data
	enum { IDD = IDR_COMMAND_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdCommandLine();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CFont		m_fontDrives;
	CStatic		m_ctrl_cmd_line;
	CHistoryComboBox	m_ctrl_combo_cmd_line;				
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelendokCmbCmdLine();
	afx_msg void OnCbnEditchangeCmbCmdLine();
	afx_msg void OnCbnEditupdateCmbCmdLine();
	afx_msg LRESULT OnComboEnterKey(WPARAM wp, LPARAM lp);
};
