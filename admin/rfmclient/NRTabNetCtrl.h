#pragma once

#include "NRListCtrlNet.h"
#include "ProcessDlg.h"
#include "c_TransportLevel.h"
#include "NRFileDataAllNet.h"
#include "DiscSelectDlg.h"
#include <vector>
#include <map>
using namespace std;

// ���������� �������������� ������
#define WM_UPDATE_LEFT_SIDE WM_USER + 5000;

typedef map<char, PATH_DISC> CArrDrives;

typedef struct __NET_WINDOWS__
{	
	__NET_WINDOWS__() : 
		chCurrentDisc('C'), pSrvConn(NULL), i_current_pos_cursor(0)
	{}
	// ������ ������� �����
	CArrDrives		 arrPath;	
	// ������ ���������� � ��������
	c_TransportLevel *pSrvConn;
	// ������� ��������� ����
	char			  chCurrentDisc;
	// ������� ������� ��� �������� ����
	int				  i_current_pos_cursor;	 
}NET_WINDOWS, *PNET_WINDOWS;

// �������� ����������

typedef vector<PNET_WINDOWS> CArrTabWindows;



// CNRTabNetCtrl

class CNRTabNetCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CNRTabNetCtrl)

public:
	CNRTabNetCtrl(CDiscSelectDlg *pSelectDriveDlg);
	virtual ~CNRTabNetCtrl();
	// �������� ����� ���������� 
	void AddNewConnect(CString strNameNet, UINT iPort);
	// ������� ��������� ����������
	void DeleteNet(int indx_net, BOOL bUpdate = TRUE);
	CString get_current_path() const {return m_sz_curent_path.IsEmpty() ? "" : m_sz_curent_path + "\\";}
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��������� ������
	void OnTabSetCursor();
	// ���������� �������� �� ��������� � ������� ������
	afx_msg LRESULT OnChangeCatalog(WPARAM wp, LPARAM lp);
	// ���������� ���������� ��������� ������
	afx_msg LRESULT OnUpdateLocalSide(WPARAM wp, LPARAM lp);
	// ���������� ����� � ��������
	// wp - ������ ����������� 
	afx_msg LRESULT OnDisconnectServer(WPARAM wp, LPARAM lp);
	// ������� �������
	//int m_nCursorItem;
	// ������������ ������ �� ��������� ������
	afx_msg LRESULT OnSwichLocalDisc(WPARAM wp, LPARAM lp);
	// ����� �������� ������
	afx_msg LRESULT OnSelecItem(WPARAM wp, LPARAM lp);
	// ������� �������������� �����
	afx_msg LRESULT OnCommandRenameFile(WPARAM wp, LPARAM lp);
	// ������� ������� �� �������
	void OnCreateDir(CString szName);
	// ���������� ����� �� ������
	void CopyFileToServer(vector<CString> arrPath);
	// ���������� ����� � �������
	void CopyFileFromServer(CString sz_local_path);
	// ����������� ����� �� ������
	void MoveFileToServer(vector<CString> arrPath);
	// ����������� ����� � �������
	void MoveFileFromServer(CString sz_local_path);
	HANDLE m_hThreadCopy;
	DWORD m_dwIDThreadCopy;
	// �������� ����� �� �������
	void OnDeleteFiles();
	// �������� ��� �� ��������� ����������
	void UpdateAllOnActive();
	// ��������� ������� �� �������
	BOOL ExecuteFile(CString strPath, CString sz_dir, CString sz_params);
	CNRListCtrl *get_list_files(){return &m_ListCtrlNet;}
private:
	// ��������� �� ����� ������ ������
	CDiscSelectDlg *m_pSelectDriveDlg;
	// ������� ������
	CImageList m_imgList;
	// ��������� ������ �� ���������������� 
	// �������� ����
	void SetActiveData(int indx);
	// ������ ��� ��� ������
	CImageList	m_tabImageList;
	// �������� ���� ��� ����������� ������ 
	CNRListCtrlNet	m_ListCtrlNet;
	// ������ ������� ������� 
	CArrTabWindows	m_arrNetWindows;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	// ����������� � �������
	BOOL ConnectToServer(c_TransportLevel *pRfmSocket, CString szServer, int port);
	// ������� ������� � ������ ������ �� ��������� �������
	BOOL LoadDataDrivesLocal(CArrDrives *parrPath, CRfmSocketArr *pArrConnectObj, int indx);
	// ��������� ����� ������ � ������ ������
	// szPathroot - ���� � �������� ��������
	BOOL LoadNewDataFile(CString szPathroot, CRfmSocketArr *pArrConnectObj, int indx);
	// ������ ������ ��� (��� �����������)
	void CreateFirstTab();
	// ������� ��������������� ����������� � �������
	UINT m_i_ids_conn;
	// ������� ��������� ����
	CString m_sz_curent_path;
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CImageList m_img_list;
};


