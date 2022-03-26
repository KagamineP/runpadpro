#pragma once

#include ".\Rfmclientsocket.h"
#include "RfmThreads.h"
//#include "NRFileData.h"


// ������������ ������ �������� ������ ��� ����������� ������
#define MAX_BUFER_COPY_FILES 2048
#define DEFAULT_ERROR "���������� ������!"

/////////////////////////////////////////////////////////////////////////////////////
// �������������� ������ �������
/////////////////////////////////////////////////////////////////////////////////////

#define ID_GET_LIST_DRIVES	0x5  // �������� ������ ������ � �������
#define ID_GET_LIST_FILES	0x7  // �������� ������ ������ �� �������� ���������� ��������
#define ID_DELETE_FILE		0xA  // �������� �����/��������
#define ID_CREATE_DIR		0xC  // ������� �����������
#define ID_COPY_IN			0xD  // ���������� ���� �� ������
#define ID_COPY_OUT			0xE  // ���������� ���� � �������		 
#define ID_MOVE_IN			0xF  // ����������� ���� �� ������	
#define ID_MOVE_OUT			0x10 // ����������� ���� � �������
#define ID_RENAME_FILE		0x11 // ������������� ����
#define ID_EXEC_FILE		0x12 // ��������� ����
#define ID_ERROR_PACK		0x15 // ��������� �����
#define ID_COUNT_FILES_DIR	0x17 // ���������� ���������� ������ � ��������


/////////////////////////////////////////////////////////////////////////////////////
// ��������� ������ ���������� ������� ����������� ������
/////////////////////////////////////////////////////////////////////////////////////

#define ERR_TIMEOUT			0x80 // ������� ����� �������� ���������� ������
#define ERR_ABORT_PACK	    0x81 // ����� ������� �������� (����� ���� �������������� ������������� ��� ����� �� ������)
#define ERR_LIST_DRIVES	    0x70 // ������ ��������� ������ ������
#define ERR_LIST_FILES	    0x71 // ������ ��������� ������ ������ �� �������� ����
#define ERR_DELETE_FILE	    0x72 // ������ �������� �����
#define ERR_CREATE_DIR	    0x75 // ������ �������� ����������� 	 
#define ERR_COPY_FILE	    0x73 // ������ ����������� �����
#define ERR_RENAME_FILE	    0x77 // ������ �������������� �����
#define ERR_EXEC_FILE	    0x78 // ������ ���������� �����


// ����������� ������ � ������

#define SPLIT_DATA 0x4


/////////////////////////////////////////////////////////////////////////////////////
// �������� ������ ������  
/////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)
typedef struct __PACKET_DATA__
{
	unsigned char		c_sign_pack;   // ������������� ������ ������ (������ 0x2A)
	unsigned short		i_size_pack;   // ������ �������� ������
	unsigned char		c_id_packet;   // ������������� ������
	unsigned char		c_code_err;    // ��� ������ (�������� ����� ���� � ��� ������ ���� id_pack == ID_ERROR_PACK)
	unsigned __int64	ui_total_size; // ����� ������ ������
	unsigned short		i_size_data;   // ������ ������� ������ ������
}PACKET_DATA, *PPACKET_DATA;


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_LogicCommand
// ��������	: ������ ����� �������� ������� ������� ��� ���� ������� �������
/////////////////////////////////////////////////////////////////////////////////////

class c_LogicCommand
{
public:
	c_LogicCommand(CRfmClientSocket *pTransport)
	{
		if(!pTransport) throw;
		m_pTransport = pTransport;
		m_h_end_operation = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		m_p_rfm_thread = NULL;
	}
	~c_LogicCommand(){}
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize) = 0;
	virtual void break_operation() = 0;
	// ���������� ������ ���������� ��������� ����������
	void set_rfm_thread(CRfmThreads *p_rfm_thread){m_p_rfm_thread = p_rfm_thread;}
protected:
	// ������ ������������� ������
	CRfmClientSocket *m_pTransport;
	// ������� ��������� �������� ��������� �������
	HANDLE m_h_end_operation;
	// �������� ������ ��������� ������
	CString m_sz_error_descr;
	// ���� ���������� ������� �������  
	bool m_flag_finish; 
	// ���������� ��������� ����������
	CRfmThreads *m_p_rfm_thread;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_get_list_drive_cmd
// ��������	: ������ ����� ��������� ������� �� ����������� ������ � �������
/////////////////////////////////////////////////////////////////////////////////////


class c_get_list_drive_cmd : public c_LogicCommand
{
public:
	c_get_list_drive_cmd(CRfmClientSocket *pTransport);
	~c_get_list_drive_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	void break_operation();
	// ���� ������� �������, ���������� �����
	// list_drives - ���������� ������ ������ � �������
	bool get_all_drives(CListDrives& list_drives, CString& sz_err_descr);
private:
	// ������ ���������� � ������� ������
	CListDrives *m_pDrivesList;
	// ������ ��� ���������� ������ �� ������
	int m_i_size_recv_data;
};



/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_get_list_files_cmd
// ��������	: ������ ����� ��������� ������� �� ����������� ������ ������
/////////////////////////////////////////////////////////////////////////////////////


class c_get_list_files_cmd : public c_LogicCommand
{
public:
	c_get_list_files_cmd(CRfmClientSocket *pTransport);
	~c_get_list_files_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	void break_operation();
	// ���� ������� �������, ���������� ����� �� ������������� ����
	// list_drives - ���������� ������ ������
	// sz_path - �������� ����
	bool get_list_files(CListFiles& list_files, CString sz_path, CString& sz_err_descr);
private:
	// ������ ���������� � ������� ������
	CListFiles *m_pFilesList;
	// ������ ��� ���������� ������ �� ������
	int m_i_size_recv_data;
};

/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_delete_file_cmd
// ��������	: ������ ����� ��������� ������� �� �������� �����
/////////////////////////////////////////////////////////////////////////////////////


class c_delete_file_cmd : public c_LogicCommand
{
public:
	c_delete_file_cmd(CRfmClientSocket *pTransport);
	~c_delete_file_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	void break_operation();
	// ������� �������� �����
	// sz_path - ���� � ���������� �����
	// sz_err_descr - �������� ��������� ������
	bool cmd_delete_file(CString sz_path, CString& sz_err_descr);
};

/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_create_dir_cmd
// ��������	: ������ ����� ��������� ������� �������� ��������
/////////////////////////////////////////////////////////////////////////////////////


class c_create_dir_cmd : public c_LogicCommand
{
public:
	c_create_dir_cmd(CRfmClientSocket *pTransport);
	~c_create_dir_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	void break_operation();
	// ������� �������� �����
	// sz_path - ���� � ���������� �����
	// sz_err_descr - �������� ��������� ������
	bool cmd_create_dir(CString str_path, CString& sz_err_descr);
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_copy_file_in
// ��������	: ������ ����� ��������� ������� ����������� ����� �� ������
/////////////////////////////////////////////////////////////////////////////////////


class c_copy_file_in : public c_LogicCommand
{
public:
	c_copy_file_in(CRfmClientSocket *pTransport);
	~c_copy_file_in(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
	// ���������� ���� 
	// sz_path_server - ���� ����� �� �������
	// sz_path_local - ���� ����� ���������
	// sz_err - �������� ������ ���� ��� ��������
	bool cmd_copy_file(CString sz_path_server, CString sz_path_local, CString& sz_err);
protected:
	// ���������� ��������� ����� �� ������
	HANDLE m_h_file_copy;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_copy_file_out
// ��������	: ������ ����� ��������� ������� ����������� ����� � �������
/////////////////////////////////////////////////////////////////////////////////////


class c_copy_file_out : public c_LogicCommand
{
public:
	c_copy_file_out(CRfmClientSocket *pTransport);
	~c_copy_file_out(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
	// ���������� ���� 
	// sz_path_server - ���� ����� �� �������
	// sz_path_local - ���� ����� ���������
	// sz_err - �������� ������ ���� ��� ��������
	bool cmd_copy_file(CString sz_path_server, CString sz_path_local, CString& sz_err);
protected:
	// ���������� ��������� ����� �� ������
	HANDLE m_h_file_copy;
	// ����� ������ �����
	ULONGLONG m_liSize;
	// ���� � ����� �� ������ ��������
	CString m_sz_path_file;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_move_file_in
// ��������	: ������ ����� ��������� ������� ����������� ����� �� ������
/////////////////////////////////////////////////////////////////////////////////////


class c_move_file_in : public c_copy_file_in
{
public:
	c_move_file_in(CRfmClientSocket *pTransport);
	~c_move_file_in(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
	// ����������� ���� 
	// sz_path_server - ���� ����� �� �������
	// sz_path_local - ���� ����� ���������
	// sz_err - �������� ������ ���� ��� ��������
	bool cmd_move_file(CString sz_path_server, CString sz_path_local, CString& sz_err);
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_move_file_out
// ��������	: ������ ����� ��������� ������� ����������� ����� �� ������
/////////////////////////////////////////////////////////////////////////////////////


class c_move_file_out : public c_copy_file_out
{
public:
	c_move_file_out(CRfmClientSocket *pTransport);
	~c_move_file_out(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
	// ����������� ���� 
	// sz_path_server - ���� ����� �� �������
	// sz_path_local - ���� ����� ���������
	// sz_err - �������� ������ ���� ��� ��������
	bool cmd_move_file(CString sz_path_server, CString sz_path_local, CString& sz_err);
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_rename_file_cmd
// ��������	: ������ ����� ��������� ������� �������������� �����
/////////////////////////////////////////////////////////////////////////////////////

class c_rename_file_cmd : public c_LogicCommand
{
public:
	c_rename_file_cmd(CRfmClientSocket *pTransport);
	~c_rename_file_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������������� ���� 
	// sz_path_old - ���������� ����
	// sz_path_new - ����� ����
	// sz_err - �������� ������ ���� ��� ��������
	bool cmd_move_file(CString sz_path_old, CString sz_path_new, CString& sz_err);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_exec_file_cmd
// ��������	: ������ ����� ��������� ������� ������� �����
/////////////////////////////////////////////////////////////////////////////////////


class c_exec_file_cmd : public c_LogicCommand
{
public:
	c_exec_file_cmd(CRfmClientSocket *pTransport);
	~c_exec_file_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
	// ��������� ��������� ����
	// sz_path_server - ���� � �����
	bool cmd_exec_file(CString sz_path_server,CString& sz_err);
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_count_files_dir
// ��������	: ���������� ���������� ������ � ��������
/////////////////////////////////////////////////////////////////////////////////////

class c_count_files_dir : public c_LogicCommand
{
public:
	c_count_files_dir(CRfmClientSocket *pTransport);
	~c_count_files_dir(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize);
	// ����� ������� �������� � ������ ���������� ����� � ��������
	// ��� ������ ������
	virtual void break_operation();
	// ���������� ��������� ������ � ��������� �������� 
	// sz_path_server - ���� � ��������
	UINT cmd_count_files_dir(CString sz_path_server);
private:
	// ��������� ��������� ������
	UINT m_i_count_files;
};