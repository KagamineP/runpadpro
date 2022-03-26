#pragma once

#include "NRFileData.h"

// ������������ ������ �������� ������ ��� ����������� ������
#define MAX_BUFER_COPY_FILES 2048

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
	c_LogicCommand(CRfmServerClient *pTransport)
	{
		if(!pTransport) throw;
		m_pTransport = pTransport;
	}
	~c_LogicCommand(){}
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize) = 0;
	virtual void break_operation() = 0;
protected:
	// ������ ������������� ������
	CRfmServerClient *m_pTransport;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_get_list_drive_cmd
// ��������	: ������ ����� ��������� ������� �� ����������� ������ � �������
/////////////////////////////////////////////////////////////////////////////////////


class c_get_list_drive_cmd : public c_LogicCommand
{
public:
	c_get_list_drive_cmd(CRfmServerClient *pTransport);
	~c_get_list_drive_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
private:
	// ����� ����� ������� ������ �� ������
	unsigned int m_i_total_size;
	// �������� ������ ���������� ������
	void ClearAllData();
	// ������ ������ ����� ������� ���������� �������� ��������� ������
	int m_indx_drive_data;
	// ���������� ����� ����� ������ �� ������
	unsigned int get_total_size_data();
	// ����������� ������������ ������ �� ������ �����
	// pDataDrive  - ���������� �� ������ �����
	// p_data_ret  - ������������ ������ ����� ��������� 
	// b_only_size - ���� true ���������� ������ � ������� ������ ������, false - ������ ������ 
	int make_data_drive(PDATA_DISC pDataDrive, char **pp_data_ret, bool b_only_size/*=false*/);
	// ������ ���������� � ������� ������
	CListDrives m_drivesList;
};



/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_get_list_files_cmd
// ��������	: ������ ����� ��������� ������� �� ����������� ������ ������
/////////////////////////////////////////////////////////////////////////////////////


class c_get_list_files_cmd : public c_LogicCommand
{
public:
	c_get_list_files_cmd(CRfmServerClient *pTransport);
	~c_get_list_files_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
private:
	// ����� ����� ������� ������ �� ������
	unsigned int m_i_total_size;
	// �������� ������ ���������� ������
	void ClearAllData();
	// ������ ������ ����� ������� ���������� �������� ��������� ������
	int m_indx_file_data;
	// ���������� ����� ����� ������ �� ������
	unsigned int get_total_size_data();
	// ����������� ������������ ������ �� ������ �����
	// pDataFile   - ���������� �� ������ �����
	// p_data_ret  - ������������ ������ ����� ��������� 
	// b_only_size - ���� true ���������� ������ � ������� ������ ������, false - ������ ������ 
	int make_data_files(PDATA_FILE pDataFile, char **pp_data_ret, bool b_only_size/*=false*/);
	// ��������� ������ ������
	CListFiles m_listFiles;
};

/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_delete_file_cmd
// ��������	: ������ ����� ��������� ������� �� �������� �����
/////////////////////////////////////////////////////////////////////////////////////


class c_delete_file_cmd : public c_LogicCommand
{
public:
	c_delete_file_cmd(CRfmServerClient *pTransport);
	~c_delete_file_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
};

/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_create_dir_cmd
// ��������	: ������ ����� ��������� ������� �������� ��������
/////////////////////////////////////////////////////////////////////////////////////


class c_create_dir_cmd : public c_LogicCommand
{
public:
	c_create_dir_cmd(CRfmServerClient *pTransport);
	~c_create_dir_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_copy_file_in
// ��������	: ������ ����� ��������� ������� ����������� ����� �� ������
/////////////////////////////////////////////////////////////////////////////////////


class c_copy_file_in : public c_LogicCommand
{
public:
	c_copy_file_in(CRfmServerClient *pTransport);
	~c_copy_file_in(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	virtual void break_operation();
protected:
	// ���������� ��������� ����� �� ������
	HANDLE h_file_copy;
	// ���� � ����������� �����
	std::string m_sz_path_file;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_copy_file_out
// ��������	: ������ ����� ��������� ������� ����������� ����� � �������
/////////////////////////////////////////////////////////////////////////////////////


class c_copy_file_out : public c_LogicCommand
{
public:
	c_copy_file_out(CRfmServerClient *pTransport);
	~c_copy_file_out(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
protected:
	// ���������� ��������� ����� �� ������
	HANDLE h_file_copy;
	// ����� ������ �����
	ULONGLONG m_liSize;
	// ������� ������� ������ � �����
	ULONGLONG m_cur_pos_file;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_move_file_in
// ��������	: ������ ����� ��������� ������� ����������� ����� �� ������
/////////////////////////////////////////////////////////////////////////////////////


class c_move_file_in : public c_copy_file_in
{
public:
	c_move_file_in(CRfmServerClient *pTransport);
	~c_move_file_in(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_move_file_out
// ��������	: ������ ����� ��������� ������� ����������� ����� �� ������
/////////////////////////////////////////////////////////////////////////////////////


class c_move_file_out : public c_copy_file_out
{
public:
	c_move_file_out(CRfmServerClient *pTransport);
	~c_move_file_out(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
private:
	// ���� � ������������� �����
	std::string m_sz_path_move;
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_rename_file_cmd
// ��������	: ������ ����� ��������� ������� �������������� �����
/////////////////////////////////////////////////////////////////////////////////////

class c_rename_file_cmd : public c_LogicCommand
{
public:
	c_rename_file_cmd(CRfmServerClient *pTransport);
	~c_rename_file_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
};


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_exec_file_cmd
// ��������	: ������ ����� ��������� ������� ������� �����
/////////////////////////////////////////////////////////////////////////////////////


class c_exec_file_cmd : public c_LogicCommand
{
public:
	c_exec_file_cmd(CRfmServerClient *pTransport);
	~c_exec_file_cmd(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
};

/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_count_files_dir
// ��������	: ���������� ���������� ������ � ��������
/////////////////////////////////////////////////////////////////////////////////////

class c_count_files_dir : public c_LogicCommand
{
public:
	c_count_files_dir(CRfmServerClient *pTransport);
	~c_count_files_dir(void);
	// �������� ������ ��� ���������� �������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void ProcessCommand(char *pData, int nSize);
	// ������ ������� ��������
	void break_operation();
};

