#pragma once

#include "NRFileData.h"
#include <vector>
using namespace std;



// CNRListCtrl

class CNRListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CNRListCtrl)

public:
	CNRListCtrl(CImageList *pimageListSys);
	virtual ~CNRListCtrl();
	// �������� ����� ������ ��� �����������
	virtual BOOL SetNewDataFiles(CNRFileData *pNewFileData, bool b_only_files=false);
	int GetCurrentIndx(){return m_nCursorItem;} 
	void SetCurrentIndx(int indx){m_nCursorItem = indx;} 
protected:
	DECLARE_MESSAGE_MAP()
	// ���������� ������� � ������ �������  
	int m_nCursorItem;
	CString m_sz_old_name,	// ������ ��� �����
			m_sz_new_name;  // ����� ��� �����
	// ������ ��� ����������� ListCtrl 
	CListFiles m_List_files;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	// ������ ���� �� �������� ���� ����������
	static int  g_nIndxField; 
	// ������������ ����������
	static bool g_bSwitchSort; 
	// ���������� ������
	void SetCursorItem();
	// ���������� ���������� ������� 
	int GetSelectItem() const {return /*m_nCursorItem*/0;}
	// �������� ��� 
	void ClearAll();
private:
	// ���� ����������� ������ ������
	bool m_b_view_only_files;
	// ���������������� ������ ��� ����������� ������ �� 
	void InitListLocalSystem();
	// ��������� ImageList
	CImageList	*m_pimageListSys;
	// ����� �����
	CFont m_fontText;
	// ����������� ����� �� ��������� �������
	void SortItemsFile(int nSubItem);
	// ������ ������ ������ ����� ���������
	// ������� ��� ���������� ������
	CListFiles::iterator m_iter_begin_files;
	// ������� ���������� ������
	void ClearOldData();
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnAccelEnter();
//	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
//	afx_msg void OnAccelEnter();
//	afx_msg void OnAccelEnter();
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};


