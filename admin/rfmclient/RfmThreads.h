#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////
// �����		: CRfmThreads
// ��������		: ������������ ��� ���������� �������� �������� � ������ � ����������� ����������� 
//				  ������� ��������. 	
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ProcessDlg.h"
#include "ReportErrorDlg.h"
#include ".\Rfmclientsocket.h"
#include <vector>
using namespace std;

class CRfmClientSocket;

typedef vector<CString> array_path; 

typedef struct __path_ident__
{
	CString sz_path;
	bool	b_dir;	
}path_ident;

typedef vector<path_ident> array_path_ident; 

//////////////////////////////////////////////////
// ��������� ���������� ��� �������� ������
//////////////////////////////////////////////////

class CRfmThreads;

typedef struct __params_thread__
{
	CString				sz_path_server;			// ���� �����������\����������� �� ������	
	array_path			*p_arr_path;			// ������ ����� � ������
	CRfmThreads			*p_rfm_threads;			// ��������� �� ����� ���������� ��������
	CRfmClientSocket	*pConnect;				// ��������� �� ������ ���������� � ��������
	array_path_ident	*p_arr_local_path;		// ������ ����� ����������� ��������
	array_path_ident	*p_arr_server_path;		// ������ ����� ����������� �� �������
}params_thread, *pparams_thread;

class CRfmThreads
{
public:
	CRfmThreads(void);
	~CRfmThreads(void);
	// ������� ����� ��������
	// arr_del_path - ������ ����� � ������
	void delete_local_files(array_path arr_del_path);
	// ������� ����� �� �������
	void delete_server_files(array_path_ident arr_path, CRfmClientSocket *pConnect); 
	// ���������� ����� �� ������
	void copy_files_to_server(CString sz_path_server, array_path arr_copy_path, CRfmClientSocket *pConnect); 
	// ���������� ����� � �������
	void copy_files_from_server(array_path_ident arr_path_local, array_path_ident arr_path_srv, CRfmClientSocket *pConnect); 
	// ���������� ���� �� ������
	// arr_move_path - ������ ����������� ������ �� ������
	void move_files_to_server(CString sz_path_server, array_path arr_move_path, CRfmClientSocket *pConnect);
	// ����������� ����� � �������
	void move_files_from_server(array_path_ident arr_path_local, array_path_ident arr_path_srv, CRfmClientSocket *pConnect);
	// ���������� ������� ������ ������
	HANDLE get_handle_start(){return m_hThreadStart;}
	// ���������� ������� �������� ������
	HANDLE get_handle_stop(){return m_hThreadStop;}
	// ���������� ������� ������ ������ �� �������
	HANDLE get_handle_was_stoped(){return m_hThreadWasStoped;}
	// ���������� ��������� �� ������� �������
	void SetTextStatusDlg(CString text_1, CString text_2);
	// ���������� ������������ ������� ��� ��������� ��������
	void SetMaxBoundProgress(ULONGLONG li_max_bound);
	// �������� ��������� ��������� ��������
	void UpdateProgress(ULONGLONG li_cur_pos);
	// ���������� ������������ ������� ��� �������� �����������
	void SetMaxBoundProgressCopy(ULONGLONG li_max_bound);
	// �������� ��������� �������� �����������
	void UpdateProgressCopy(ULONGLONG li_cur_pos);
	// ������� ���� �� ��������� ��������
	void CloseStatusWindow();
	// �������� �������� ��������� ������
	void add_new_error(CString str_error)
	{
		m_report_errors_dlg.add_new_error(str_error);
	}
	// ���������� ���� ����������� ��������
	bool get_flag_global_stop() const {return m_global_stop;}
	// ���������� ���� ����������� ��������
	void set_flag_global_stop(bool b_flag_stop){m_global_stop = b_flag_stop;}
private:
	// ���� ��� ����������� ��������
	CProcessDlg m_proc_dlg;
	// ���������� ������� ������ ������
	HANDLE m_hThreadStart;
	// ���������� ������� �������� ������
	HANDLE m_hThreadStop;
	// ���������� ������� ������ ������ �� �������
	HANDLE m_hThreadWasStoped;
	// ���������� ������
	HANDLE m_h_thread;
	// ������������� ������
	DWORD m_dw_id_thread;
	// ������ ������ �������
	CRITICAL_SECTION m_cs_data_status;
	// ���� ������������ ����� �� �������
	CReportErrorDlg m_report_errors_dlg;
	// ���������� ������� ��������
	bool m_global_stop;
public:	
	// ���������� ����� ��� ���� ����������� �������
	// ��� ������ ���������� ���� ������� ��� ��������� ������� ����������
	static char			g_sz_text1[MAX_PATH];	// ������ ������ �� ���� �������
	static char			g_sz_text2[MAX_PATH];	// ������ ������ �� ���� �������
	static ULONGLONG	g_li_max_bound;			// ������������ ������� ��������� ��������
	static ULONGLONG	g_li_cur_pos;			// ������� ������� ��������� �������� 
	static ULONGLONG	g_li_max_bound_copy;	// ������������ ������� �������� �����������
	static ULONGLONG	g_li_cur_pos_copy;		// ������� ������� �������� �����������
};

