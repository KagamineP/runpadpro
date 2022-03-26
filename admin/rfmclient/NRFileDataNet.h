#pragma once

#include "NRFileData.h"
//#include ".\Rfmclientsocket.h"


///////////////////////////////////////////////////////////////////////
// �����		: CNRFileData
// ��������		: ������������ ��� ������ � �������, � �� ������� 
//				  �� �������	
///////////////////////////////////////////////////////////////////////

class CNRFileDataNet : public CNRFileData
{
public:
	CNRFileDataNet(CRfmClientSocket *pConnectObj);
	~CNRFileDataNet(void);
	// �������� ������ �� ��������� ������
	virtual bool GetDataFiles(CListFiles& files_list, CRfmThreads *p_proc_thread); 
	// ���������� ������ ������ �� ������
	// discList - ������ ���������� � ����� ������ ����������
	virtual bool GetDiscList(CListDrives& discList, CString& sz_err_descr);
	// ������� ����� ��� ������� ������ �� �������
	bool delete_files(CString str_path, bool b_dir, CRfmThreads *p_proc_thread);
	// ���������� ������� ����
	void SetPathRoot(CString szPathroot){m_szPathroot = szPathroot;}
	// ������� ������� �� �������
	// str_name - �������� ��������
	bool create_dir(CString str_name, CString& sz_err_descr);
	// ���������� ���� �� ������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool copy_file_to_server(CString sz_path_server, CString sz_path_local, CRfmThreads *p_proc_thread);
	// ���������� ���� c �������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool copy_file_from_server(CString sz_path_server, CString sz_path_local, bool b_dir, CRfmThreads *p_rfm_threads);
	// ����������� ���� �� ������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool move_file_to_server(CString sz_path_server, CString sz_path_local, CRfmThreads *p_proc_thread);
	// ����������� ���� c �������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool move_file_from_server(CString sz_path_server, CString sz_path_local, bool b_dir, CRfmThreads *p_proc_thread);
	// ��������� ���� �� �������
	// sz_path_server - ���� � ������������ �����
	bool execute_file_server(CString sz_path_server, CString sz_command_line, CString& sz_err);
	// ���������� ������ ���������� � ��������
	void SetConnect(CRfmClientSocket *pConnectObj){m_pConnectObj = pConnectObj;}
	// ������������� ���� �� �������
	// sz_old_path - �������������� ����
	// sz_new_path - ����� ���� 
	bool rename_file(CString sz_old_path, CString sz_new_path, CString& sz_err); 
	// ���������� ���������� ������ � �������� �� �������
	// str_path - ���� � �������� �� ������� 
	UINT get_count_file_dir_srv(CString str_path);
private:
	// ������� ���� ��� ������� �� ������
	bool delete_file(CString str_path, CString& sz_err_descr);
	// ���������� ������� ����������
	// sz_path_server - ���� �� ������� 
	// sz_path_local - ��������� ����
	// sz_name_dir - �������� ��������
	bool copy_dir_to_server(CString sz_path_server, CString sz_path_local, CString sz_name_dir, CRfmThreads *p_proc_thread, bool b_move=false);
	// ���������� ���� ���� �� ������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool copy_one_file_to_server(CString sz_path_server, CString sz_path_local, CString& sz_err, CRfmThreads *p_proc_thread);
	// ����������� ���� ���� �� ������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool move_one_file_to_server(CString sz_path_server, CString sz_path_local, CString& sz_err, CRfmThreads *p_proc_thread);
	// ���������� ���� ���� � �������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool copy_one_file_from_server(CString sz_path_server, CString sz_path_local, CString& sz_err, CRfmThreads *p_rfm_threads);
	// ����������� ���� ���� � �������
	// sz_path_server - ���� ����������� �� �������
	// sz_path_local - ���� ����������� ���������
	bool move_one_file_from_server(CString sz_path_server, CString sz_path_local, CString& sz_err, CRfmThreads *p_proc_thread);
	// ��� ������������� ��������
	int m_codeFunc; 
	// ��������� ������ � �������
	char *m_pDataIn;
	// ������ ��������� ������ � �������
	int m_sizeDataIn;
	// ������� ��������� ��������
	HANDLE m_hEndProccess;
	// ������ ����������� �������
	UINT m_iStatusCommand;
protected:
	// ������� ���� ��� ������ ������ �� �������
	CString m_szPathroot;
	// ������ ���������� � ��������
	CRfmClientSocket *m_pConnectObj;
};

