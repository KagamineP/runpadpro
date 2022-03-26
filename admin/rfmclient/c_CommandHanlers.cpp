#include "StdAfx.h"
#include "c_CommandHanlers.h"
#include "c_TransportLevel.h"
#include "nrfiledata.h"

// ������������ ����� �������� ������ � �������

#define MAX_TIME_WAIT INFINITE
 

//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "�������� ���������� � ������ �� ��������� �������"
//////////////////////////////////////////////////////////////////////////////////////////

c_get_list_drive_cmd::c_get_list_drive_cmd(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
	m_i_size_recv_data = 0;
}

c_get_list_drive_cmd::~c_get_list_drive_cmd(void)
{
}


// ���� ������� �������, ���������� �����
// list_drives - ���������� ������ ������ � �������

bool c_get_list_drive_cmd::get_all_drives(CListDrives& list_drives, CString& sz_err_descr)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_i_size_recv_data = 0; 

	if(!m_pTransport->is_connected())
	{
		sz_err_descr = "��� ����� � ��������!";
		return false;
	}
	
	
	// �������� ��������� �� ������ � �������

	m_pDrivesList = &list_drives;
	
	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_GET_LIST_DRIVES, 0, 0L, 0, NULL, &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err_descr = "��������� ����� �������� ������ � �������!";
		m_i_size_recv_data = 0; 
		return false;
	}

	sz_err_descr = m_sz_error_descr;
	m_i_size_recv_data = 0; 
	return m_pDrivesList->size() != 0;
}


// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������


void c_get_list_drive_cmd::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_i_size_recv_data = 0; 
	::SetEvent(m_h_end_operation);
}

// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_get_list_drive_cmd::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
	{
		m_sz_error_descr = "���������� �������� ������ �� ������ � �������, ������ �� ��������!";
		::SetEvent(m_h_end_operation);
		return;
	}
	
	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;
	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		PDATA_DISC p_data_drive = new DATA_DISC;
		unsigned short i_tmp_size = 0;

		// ������������ ���������� ������

		p_data_drive->szLetterDisc = *pdata_pack++;
		memcpy(&p_data_drive->iTypeDisc, pdata_pack, sizeof(unsigned int)); 
		pdata_pack += sizeof(unsigned int);
		memcpy(&i_tmp_size, pdata_pack, sizeof(unsigned short)); 
		pdata_pack += sizeof(unsigned short);
		p_data_drive->szLabel.Append(pdata_pack, i_tmp_size);
		pdata_pack += i_tmp_size;
		memcpy(&i_tmp_size, pdata_pack, sizeof(unsigned short)); 
		pdata_pack += sizeof(unsigned short);
		p_data_drive->szFileSys.Append(pdata_pack, i_tmp_size);
		pdata_pack += i_tmp_size;
		memcpy(&p_data_drive->d_free_space, pdata_pack, sizeof(double)); 
		pdata_pack += sizeof(double);	

		// �������� � ������ ����� ����
		m_i_size_recv_data += pPacket->i_size_data;
		m_pDrivesList->push_back(p_data_drive);

		// ������ ��� �� ��� ������� ������ � ���� ��� �� �������� 
		// ������ �� ��������� ����
		
		if(m_i_size_recv_data < pPacket->ui_total_size)
		{
			char *p_packet = NULL;
			int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_GET_LIST_DRIVES, 0, 
				m_i_size_recv_data, 0, NULL, &p_packet);	
			((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
			if(p_packet) delete [] p_packet;
		}
		else
			// ����� �������� ������ �� ���������� ������
			::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_i_size_recv_data = 0; 			
		::SetEvent(m_h_end_operation);
		return;
	}
	else
	if(pPacket->c_code_err == ERR_LIST_DRIVES)
	{
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_i_size_recv_data = 0; 			
		::SetEvent(m_h_end_operation);
	}
	else
		return;

}




//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "�������� ������ ������ �� ���������� ����"
//////////////////////////////////////////////////////////////////////////////////////////

c_get_list_files_cmd::c_get_list_files_cmd(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
	m_i_size_recv_data = 0;
}

c_get_list_files_cmd::~c_get_list_files_cmd(void)
{
}

// ���� ������� �������, ���������� ����� �� ������������� ����
// list_drives - ���������� ������ ������
// sz_path - �������� ����

bool c_get_list_files_cmd::get_list_files(CListFiles& list_files, CString sz_path, CString& sz_err_descr)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_i_size_recv_data = 0; 
	m_flag_finish = false;

	if(!m_pTransport->is_connected())
	{
		sz_err_descr = "��� ����� � ��������!";
		return false;
	}
	
	// �������� ��������� �� ������ � �������

	m_pFilesList = &list_files;
	
	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_GET_LIST_FILES, 0, 0L, sz_path.GetLength()+1, 
		sz_path.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err_descr = "��������� ����� �������� ������ � �������!";
		m_i_size_recv_data = 0; 
		return false;
	}
	
	sz_err_descr = m_sz_error_descr;
	m_i_size_recv_data = 0; 
	
	return m_flag_finish;
}



// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_get_list_files_cmd::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_i_size_recv_data = 0; 
	m_flag_finish = false;
	::SetEvent(m_h_end_operation);
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_get_list_files_cmd::ProcessCommand(char *pData, int nSize)
{
	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;
	
	// ������� ���������� �������� ������
		
	if(m_p_rfm_thread)
	{
		if(WaitForSingleObject(m_p_rfm_thread->get_handle_stop(), 0) == WAIT_OBJECT_0)
		{
			// �������� �� ������ ����� ������� �������� 
			
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_GET_LIST_FILES, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;			
		
			// ��������� ��� ���������� �����
			
			for(size_t i = 0; i < m_pFilesList->size(); i++)
			{
				if((*m_pFilesList)[i])
					delete (*m_pFilesList)[i];
			}
			m_pFilesList->clear();

			// ���������� ������� ������
		
			m_flag_finish = false; 
			::SetEvent(m_h_end_operation);	
			return;
		}
	}

	if(!pData || !nSize)
	{
		m_sz_error_descr = "���������� �������� ������ �� ������ � �������!";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		PDATA_FILE p_data_file = new DATA_FILE;
		unsigned short i_tmp_size = 0;

		// ������������ ���������� ������

		memcpy(&i_tmp_size, pdata_pack, sizeof(unsigned short)); 
		pdata_pack += sizeof(unsigned short);
		p_data_file->szNameFile.Append(pdata_pack, i_tmp_size);
		pdata_pack += i_tmp_size;
		p_data_file->bDirectory = (BOOL)*pdata_pack++; 
		p_data_file->bDots = (BOOL)*pdata_pack++; 
		memcpy(&p_data_file->iSizeFile, pdata_pack, sizeof(ULONGLONG));
		pdata_pack += sizeof(ULONGLONG);
		FILETIME file_time;
		memcpy(&file_time, pdata_pack, sizeof(FILETIME));
		p_data_file->timeCreate = (CTime)file_time;
		
		// �������� � ������ ����� ����
		m_i_size_recv_data += pPacket->i_size_data;
		m_pFilesList->push_back(p_data_file);

		// ������ ��� �� ��� ������� ������ � ���� ��� �� �������� 
		// ������ �� ��������� ����
		
		if(m_i_size_recv_data < pPacket->ui_total_size)
		{
			char *p_packet = NULL;
			int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_GET_LIST_FILES, 0, 
				m_i_size_recv_data, 0, NULL, &p_packet);	
			((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
			if(p_packet) delete [] p_packet;
		}
		else
		{
			m_flag_finish = true; 
			// ����� �������� ������ �� ���������� ������
			::SetEvent(m_h_end_operation);
		}
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_i_size_recv_data = 0; 
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	else
	if(pPacket->c_code_err == ERR_LIST_FILES)
	{
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_i_size_recv_data = 0; 
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}



//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "������� ���� �� �������"
//////////////////////////////////////////////////////////////////////////////////////////

c_delete_file_cmd::c_delete_file_cmd(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
}

c_delete_file_cmd::~c_delete_file_cmd(void)
{
}

// ������� �������� �����
// sz_path - ���� � ���������� �����
// sz_err_descr - �������� ��������� ������

bool c_delete_file_cmd::cmd_delete_file(CString sz_path, CString& sz_err_descr)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;

	if(!m_pTransport->is_connected())
	{
		sz_err_descr = "��� ����� � ��������!";
		return false;
	}

	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_DELETE_FILE, 0, 0L, sz_path.GetLength()+1, 
		sz_path.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err_descr = "��������� ����� �������� ������ � �������!";
		return false;
	}
	
	sz_err_descr = m_sz_error_descr;
	
	return m_flag_finish;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_delete_file_cmd::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	::SetEvent(m_h_end_operation);
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_delete_file_cmd::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
	{
		m_sz_error_descr = DEFAULT_ERROR;
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		m_sz_error_descr.Empty();
		m_flag_finish = true; 
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	else
	if(pPacket->c_code_err == ERR_DELETE_FILE)
	{
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "������� ������� �� �������"
//////////////////////////////////////////////////////////////////////////////////////////

c_create_dir_cmd::c_create_dir_cmd(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
}

c_create_dir_cmd::~c_create_dir_cmd(void)
{
}


// ������� �������� �����
// sz_path - ���� � ���������� �����
// sz_err_descr - �������� ��������� ������

bool c_create_dir_cmd::cmd_create_dir(CString str_path, CString& sz_err_descr)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;

	if(!m_pTransport->is_connected())
	{
		sz_err_descr = "��� ����� � ��������!";
		return false;
	}

	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_CREATE_DIR, 0, 0L, str_path.GetLength()+1, 
		str_path.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err_descr = "��������� ����� �������� ������ � �������!";
		return false;
	}
	
	sz_err_descr = m_sz_error_descr;
	
	return m_flag_finish;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_create_dir_cmd::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	::SetEvent(m_h_end_operation);
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_create_dir_cmd::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
	{
		m_sz_error_descr = DEFAULT_ERROR;
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		m_sz_error_descr.Empty();
		m_flag_finish = true; 
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	else
	if(pPacket->c_code_err == ERR_CREATE_DIR)
	{
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}


//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "���������� ���� �� ������"
//////////////////////////////////////////////////////////////////////////////////////////

c_copy_file_in::c_copy_file_in(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
	m_h_file_copy = NULL;
}

c_copy_file_in::~c_copy_file_in(void)
{
}


// ���������� ���� 
// sz_path_server - ���� ����� �� �������
// sz_path_local - ���� ����� ���������
// sz_err - �������� ������ ���� ��� ��������

bool c_copy_file_in::cmd_copy_file(CString sz_path_server, CString sz_path_local, CString& sz_err)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE && m_h_file_copy != NULL){ 
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = INVALID_HANDLE_VALUE;
	}

	if(!m_pTransport->is_connected())
	{
		sz_err = "��� ����� � ��������!";
		return false;
	}

	// ������� ���� ��� �����������

	m_h_file_copy = ::CreateFile(sz_path_local, GENERIC_READ, 0, NULL, 
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_h_file_copy == INVALID_HANDLE_VALUE)
	{
		sz_err = "���������� ������� ���� �� �����������!";
		return false;
	}
	
	// ���������� ��������� ���������
	
	DWORD dw_hight = 0;
	ULONGLONG li_max_bound = ::GetFileSize(m_h_file_copy, &dw_hight);
	li_max_bound |= (dw_hight << sizeof(unsigned int));
	m_p_rfm_thread->SetMaxBoundProgressCopy(li_max_bound);
	
	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_IN, 0, 0L, sz_path_server.GetLength()+1, 
		sz_path_server.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err = "��������� ����� �������� ������ � �������!";
		if(m_h_file_copy != INVALID_HANDLE_VALUE) ::CloseHandle(m_h_file_copy);
		return false;
	}
	
	sz_err = m_sz_error_descr;
	
	return m_flag_finish;
}


// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_copy_file_in::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE){
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
	}
	::SetEvent(m_h_end_operation);
}

// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_copy_file_in::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
		return;
	
	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;

	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		DWORD read_data=0;
		char ch_tmp_buf[MAX_BUFER_COPY_FILES];

		// ������ ������ �����

		DWORD dwHight = 0;	
		ULONGLONG iSizeFile = (ULONGLONG)::GetFileSize(m_h_file_copy, &dwHight);
		iSizeFile |= ((ULONGLONG)dwHight << 32); 

		// ���� ���� � ������� ������

		if(!iSizeFile)
		{
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_IN, 0, 
			1, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
		}

		// ������ �� ���� �� ����������

		if(pPacket->ui_total_size >= iSizeFile)
		{
			
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_flag_finish = true; 
			::SetEvent(m_h_end_operation);
			return;
		}

		// ������� ���������� �������� ������
		
		if(WaitForSingleObject(m_p_rfm_thread->get_handle_stop(), 0) == WAIT_OBJECT_0)
		{
			// �������� �� ������ ����� ������� �������� 
			
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_IN, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;			
		
			// ���������� ������� ������
		
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_flag_finish = true; 
			::SetEvent(m_h_end_operation);	
			return;
		}


		if(!::ReadFile(m_h_file_copy, ch_tmp_buf, MAX_BUFER_COPY_FILES, &read_data, NULL))
		{
			DWORD dw_err = ::GetLastError();
			
			
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_IN, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_sz_error_descr = "������ ������ �����!";
			m_flag_finish = false; 
			::SetEvent(m_h_end_operation);
			return;
		}
	
		// �������� ������ �������

		i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_IN, 0, 
		iSizeFile, (unsigned short)read_data, ch_tmp_buf, &p_data_packet);
		((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
		if(p_data_packet) delete [] p_data_packet;			
	
		// �������� ��������� ��������
		
		m_p_rfm_thread->UpdateProgressCopy(i_size);
	}
	else
	if(pPacket->c_code_err == ERR_COPY_FILE)
	{
		CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}



//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "���������� ���� � �������"
//////////////////////////////////////////////////////////////////////////////////////////

c_copy_file_out::c_copy_file_out(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
	m_h_file_copy = NULL;
	m_liSize = 0;
	m_sz_path_file = "";
}

c_copy_file_out::~c_copy_file_out(void)
{
}




// ���������� ���� 
// sz_path_server - ���� ����� �� �������
// sz_path_local - ���� ����� ���������
// sz_err - �������� ������ ���� ��� ��������

bool c_copy_file_out::cmd_copy_file(CString sz_path_server, CString sz_path_local, CString& sz_err)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE){
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
	}

	if(!m_pTransport->is_connected())
	{
		sz_err = "��� ����� � ��������!";
		return false;
	}

	SetFileAttributes(sz_path_local, FILE_ATTRIBUTE_NORMAL);
	
	// ������� ���� ��� �����������

	m_h_file_copy = ::CreateFile(sz_path_local, GENERIC_WRITE, 0, NULL, 
					CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_h_file_copy == INVALID_HANDLE_VALUE)
	{
		sz_err = "���������� ������� ���� �� �����������!";
		return false;
	}
	
	m_sz_path_file = sz_path_local;

	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_OUT, 0, 
		(sz_path_server.GetLength()+1 + ID_COPY_OUT), // ����������� ������������� ������ �������� ����� 
		sz_path_server.GetLength()+1, 
		sz_path_server.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err = "��������� ����� �������� ������ � �������!";
		if(m_h_file_copy != INVALID_HANDLE_VALUE)::CloseHandle(m_h_file_copy);
		return false;
	}
	
	sz_err = m_sz_error_descr;
	
	return m_flag_finish;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_copy_file_out::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE){
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
	}
	::DeleteFile(m_sz_path_file);
	::SetEvent(m_h_end_operation);
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_copy_file_out::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
		return;
	
	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;
	char *p_packet = NULL;
	int size_pack = 0;

	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		if(m_liSize != pPacket->ui_total_size)
		{
			m_liSize = pPacket->ui_total_size;
			
			// ������������� �� �� ��� ���� �� ������� ��� ������
			// ��������� ������ ������ ������ � �������
				
			size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_OUT, 0, 
				0, 0, NULL, &p_packet);	
			((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
			if(p_packet) delete [] p_packet;

			// ���������� ������� ��������

			m_p_rfm_thread->SetMaxBoundProgressCopy(m_liSize);

			return; 
		}
		
		// ���������� ������� ����
			
		DWORD dw_was_write = 0;
		if(!::WriteFile(m_h_file_copy, pdata_pack, pPacket->i_size_data, &dw_was_write, NULL))
		{
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_OUT, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
			::CloseHandle(m_h_file_copy);
			::DeleteFile(m_sz_path_file);
			m_h_file_copy = NULL;
			m_sz_path_file.Empty();
			m_liSize = 0;
			m_flag_finish = false; 
			::SetEvent(m_h_end_operation);
			return;
		}
		
		// �������� �������

		m_p_rfm_thread->UpdateProgressCopy(pPacket->i_size_data);

		// ������� ���������� �������� ������
		
		if(WaitForSingleObject(m_p_rfm_thread->get_handle_stop(), 0) == WAIT_OBJECT_0)
		{
			// �������� �� ������ ����� ������� �������� 
			
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_OUT, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;			
		
			// ���������� ������� ������
		
			::CloseHandle(m_h_file_copy);
			::DeleteFile(m_sz_path_file);
			m_h_file_copy = NULL;
			m_flag_finish = true; 
			m_liSize = 0;
			::SetEvent(m_h_end_operation);	
			return;
		}
		
		
		// �������� ������� ����� ����� �������
			
		DWORD dwHight = 0;	
		ULONGLONG iCurSize = (ULONGLONG)::GetFileSize(m_h_file_copy, &dwHight);
		iCurSize |= ((ULONGLONG)dwHight << 32); 
					
		// ������ �� ���� �� ��� ����������

		if(iCurSize == m_liSize)
		{
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_sz_path_file.Empty();
			m_liSize = 0;
			m_flag_finish = true; 
			::SetEvent(m_h_end_operation);

		}
		else
		{
			// �������� ������� ��� ���� ������� ������
		
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COPY_OUT, 0, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
		}
	}
	else
	if(pPacket->c_code_err == ERR_COPY_FILE)
	{
		::CloseHandle(m_h_file_copy);
		::DeleteFile(m_sz_path_file);
		m_h_file_copy = NULL;
		m_sz_path_file.Empty();
		m_liSize = 0;
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		::CloseHandle(m_h_file_copy);
		::DeleteFile(m_sz_path_file);
		m_h_file_copy = NULL;
		m_sz_path_file.Empty();
		m_liSize = 0;
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}



//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "����������� ���� �� ������"
//////////////////////////////////////////////////////////////////////////////////////////

c_move_file_in::c_move_file_in(CRfmClientSocket *pTransport) :
	c_copy_file_in(pTransport)
{
}

c_move_file_in::~c_move_file_in(void)
{
}

// ����������� ���� 
// sz_path_server - ���� ����� �� �������
// sz_path_local - ���� ����� ���������
// sz_err - �������� ������ ���� ��� ��������

bool c_move_file_in::cmd_move_file(CString sz_path_server, CString sz_path_local, CString& sz_err)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = INVALID_HANDLE_VALUE;
	}
	if(!m_pTransport->is_connected())
	{
		sz_err = "��� ����� � ��������!";
		return false;
	}

	SetFileAttributes(sz_path_local, FILE_ATTRIBUTE_NORMAL);

	// ������� ���� ��� �����������

	m_h_file_copy = ::CreateFile(sz_path_local, GENERIC_READ, 0, NULL, 
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_h_file_copy == INVALID_HANDLE_VALUE)
	{
		sz_err = "���������� ������� ���� �� �����������!";
		return false;
	}
	
	// ���������� ��������� ���������
	
	DWORD dw_hight = 0;
	ULONGLONG li_max_bound = ::GetFileSize(m_h_file_copy, &dw_hight);
	li_max_bound |= ((ULONGLONG)dw_hight << (sizeof(unsigned int)*8));
	m_p_rfm_thread->SetMaxBoundProgressCopy(li_max_bound);	

	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_IN, 0, 0L, sz_path_server.GetLength()+1, 
		sz_path_server.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������
	

	::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
		
	sz_err = m_sz_error_descr;
	
	BOOL bts; 
	if(m_flag_finish)
		bts = ::DeleteFile(sz_path_local);
	
	return m_flag_finish;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_move_file_in::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE){
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = INVALID_HANDLE_VALUE;
	}
	::SetEvent(m_h_end_operation);
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_move_file_in::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
		return;
	

	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;

	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		DWORD read_data=0;
		char ch_tmp_buf[MAX_BUFER_COPY_FILES];

		if(WaitForSingleObject(m_p_rfm_thread->get_handle_stop(), 0) == WAIT_OBJECT_0)
		{
			// �������� �� ������ ����� ������� �������� 
			
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_IN, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;			
		
			// ���������� ������� ������
		
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_flag_finish = false; 
			m_h_file_copy = INVALID_HANDLE_VALUE;
			::SetEvent(m_h_end_operation);	
			return;
		}


		// ������ ������ �����

		DWORD dwHight = 0;	
		ULONGLONG iSizeFile = (ULONGLONG)::GetFileSize(m_h_file_copy, &dwHight);
		iSizeFile |= ((ULONGLONG)dwHight << 32); 

		// ������ �� ���� �� ����������

		if(pPacket->ui_total_size >= iSizeFile)
		{
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_flag_finish = true; 
			m_h_file_copy = INVALID_HANDLE_VALUE;
			::SetEvent(m_h_end_operation);
			return;
		}


		if(!::ReadFile(m_h_file_copy, ch_tmp_buf, MAX_BUFER_COPY_FILES, &read_data, NULL))
		{
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_IN, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_sz_error_descr = "������ ������ �����!";
			m_flag_finish = false; 
			::SetEvent(m_h_end_operation);
			return;
		}
	
		// �������� ������ �������

		i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_IN, 0, 
		iSizeFile, (unsigned short)read_data, ch_tmp_buf, &p_data_packet);
		((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
		if(p_data_packet) delete [] p_data_packet;		

		m_p_rfm_thread->UpdateProgressCopy(read_data);
	
	}
	else
	if(pPacket->c_code_err == ERR_COPY_FILE)
	{
		CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		m_h_file_copy = INVALID_HANDLE_VALUE;
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		CloseHandle(m_h_file_copy);
		m_h_file_copy = NULL;
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		m_h_file_copy = INVALID_HANDLE_VALUE;
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}


//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "����������� ���� � �������"
//////////////////////////////////////////////////////////////////////////////////////////

c_move_file_out::c_move_file_out(CRfmClientSocket *pTransport) :
	c_copy_file_out(pTransport)
{
}

c_move_file_out::~c_move_file_out(void)
{
}

// ����������� ���� 
// sz_path_server - ���� ����� �� �������
// sz_path_local - ���� ����� ���������
// sz_err - �������� ������ ���� ��� ��������

bool c_move_file_out::cmd_move_file(CString sz_path_server, CString sz_path_local, CString& sz_err)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = INVALID_HANDLE_VALUE;
	}

	if(!m_pTransport->is_connected())
	{
		sz_err = "��� ����� � ��������!";
		return false;
	}

	SetFileAttributes(sz_path_local, FILE_ATTRIBUTE_NORMAL);

	// ������� ���� ��� �����������

	m_h_file_copy = ::CreateFile(sz_path_local, GENERIC_WRITE, 0, NULL, 
					CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_h_file_copy == INVALID_HANDLE_VALUE)
	{
		sz_err = "���������� ������� ���� �� �����������!";
		return false;
	}
	
	m_sz_path_file = sz_path_local;

	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_OUT, 0, 
		(sz_path_server.GetLength()+1 + ID_MOVE_OUT), // ����������� ������������� ������ �������� ����� 
		sz_path_server.GetLength()+1, 
		sz_path_server.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err = "��������� ����� �������� ������ � �������!";
		if(m_h_file_copy != INVALID_HANDLE_VALUE){
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = INVALID_HANDLE_VALUE;
		}
		return false;
	}
	
	sz_err = m_sz_error_descr;
	
	return m_flag_finish;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_move_file_out::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	if(m_h_file_copy != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_h_file_copy);
		m_h_file_copy = INVALID_HANDLE_VALUE;
	}
	::SetEvent(m_h_end_operation);
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_move_file_out::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
		return;
	
	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;
	char *p_packet = NULL;
	int size_pack = 0;

	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		if(m_liSize != pPacket->ui_total_size)
		{
			m_liSize = pPacket->ui_total_size;
			
			// ������������� �� �� ��� ���� �� ������� ��� ������
			// ��������� ������ ������ ������ � �������
				
			size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_OUT, 0, 
				0, 0, NULL, &p_packet);	
			((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
			if(p_packet) delete [] p_packet;

			// ���������� ������� ��������

			m_p_rfm_thread->SetMaxBoundProgressCopy(m_liSize);	

			return; 
		}
		
		// ���������� ������� ����
			
		DWORD dw_was_write = 0;
		if(!::WriteFile(m_h_file_copy, pdata_pack, pPacket->i_size_data, &dw_was_write, NULL))
		{
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_OUT, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
			::CloseHandle(m_h_file_copy);
			::DeleteFile(m_sz_path_file);
			m_h_file_copy = NULL;
			m_sz_path_file.Empty();
			m_liSize = 0;
			m_flag_finish = false; 
			::SetEvent(m_h_end_operation);
			return;
		}
		
		// ������� ���������� �������� ������
		
		if(WaitForSingleObject(m_p_rfm_thread->get_handle_stop(), 0) == WAIT_OBJECT_0)
		{
			// �������� �� ������ ����� ������� �������� 
			
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_OUT, ERR_ABORT_PACK, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;			
		
			// ���������� ������� ������
		
			::CloseHandle(m_h_file_copy);
			::DeleteFile(m_sz_path_file);
			m_h_file_copy = NULL;
			m_flag_finish = true; 
			::SetEvent(m_h_end_operation);	
			return;
		}


		// �������� ������� ����� ����� �������
			
		DWORD dwHight = 0;	
		ULONGLONG iCurSize = (ULONGLONG)::GetFileSize(m_h_file_copy, &dwHight);
		iCurSize |= ((ULONGLONG)dwHight << 32); 
					
		// ������ �� ���� �� ��� ����������

		if(iCurSize == m_liSize)
		{
			::CloseHandle(m_h_file_copy);
			m_h_file_copy = NULL;
			m_sz_path_file.Empty();
			m_liSize = 0;
			m_flag_finish = true; 
			::SetEvent(m_h_end_operation);
		}
		else
		{
			// �������� ������� ��� ���� ������� ������
		
			i_size = ((c_TransportLevel*)m_pTransport)->make_packet(ID_MOVE_OUT, 0, 
			0, 0, NULL, &p_data_packet);
			((c_TransportLevel*)m_pTransport)->SendData(p_data_packet, i_size);
			if(p_data_packet) delete [] p_data_packet;
		}
	
		m_p_rfm_thread->UpdateProgressCopy(dw_was_write);
	}
	else
	if(pPacket->c_code_err == ERR_COPY_FILE)
	{
		::CloseHandle(m_h_file_copy);
		::DeleteFile(m_sz_path_file);
		m_h_file_copy = NULL;
		m_sz_path_file.Empty();
		m_liSize = 0;
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		::CloseHandle(m_h_file_copy);
		::DeleteFile(m_sz_path_file);
		m_h_file_copy = NULL;
		m_sz_path_file.Empty();
		m_liSize = 0;
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		
	}
	else
		return;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "������������� ����"
//////////////////////////////////////////////////////////////////////////////////////////

c_rename_file_cmd::c_rename_file_cmd(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
}

c_rename_file_cmd::~c_rename_file_cmd(void)
{
}

// ������������� ���� 
// sz_path_old - ���������� ����
// sz_path_new - ����� ����
// sz_err - �������� ������ ���� ��� ��������

bool c_rename_file_cmd::cmd_move_file(CString sz_path_old, CString sz_path_new, CString& sz_err)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;

	if(!m_pTransport->is_connected())
	{
		sz_err = "��� ����� � ��������!";
		return false;
	}

	CString sz_data_path = sz_path_old + '\04' + sz_path_new;	

	// �������� ������� ������� 

	char *p_packet = NULL;
	
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_RENAME_FILE, 0, 0L, sz_data_path.GetLength()+1, 
		sz_data_path.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err = "��������� ����� �������� ������ � �������!";
		return false;
	}
	
	sz_err = m_sz_error_descr;
	
	return m_flag_finish;
}

// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_rename_file_cmd::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
	{
		m_sz_error_descr = DEFAULT_ERROR;
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		m_sz_error_descr.Empty();
		m_flag_finish = true; 
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	else
	if(pPacket->c_code_err == ERR_RENAME_FILE)
	{
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_rename_file_cmd::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	::SetEvent(m_h_end_operation);
}



//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "��������� ����"
//////////////////////////////////////////////////////////////////////////////////////////

c_exec_file_cmd::c_exec_file_cmd(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
}

c_exec_file_cmd::~c_exec_file_cmd(void)
{
}

// ��������� ��������� ����
// sz_path_server - ���� � �����

bool c_exec_file_cmd::cmd_exec_file(CString sz_path_server, CString& sz_err)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;

	if(!m_pTransport->is_connected())
	{
		sz_err = "��� ����� � ��������!";
		return false;
	}

	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_EXEC_FILE, 0, 0L, sz_path_server.GetLength()+1, 
		sz_path_server.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		sz_err = "��������� ����� �������� ������ � �������!";
		return false;
	}
	
	sz_err = m_sz_error_descr;
	
	return m_flag_finish;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������



void c_exec_file_cmd::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
}


// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_exec_file_cmd::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
	{
		m_sz_error_descr = DEFAULT_ERROR;
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA);

	if(pPacket->c_code_err == 0)
	{
		m_sz_error_descr.Empty();
		m_flag_finish = true; 
		::SetEvent(m_h_end_operation);
	}
	else
	if(pPacket->c_code_err == ERR_TIMEOUT)
	{
		m_sz_error_descr = "��������� ����� �������� ������ � �������";
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
		return;
	}
	else
	if(pPacket->c_code_err == ERR_EXEC_FILE)
	{
		m_sz_error_descr.Append(pdata_pack, pPacket->i_size_data);
		m_flag_finish = false; 
		::SetEvent(m_h_end_operation);
	}
	else
		return;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ����� ��������� ������� "���������� ���������� ������ � ��������"
//////////////////////////////////////////////////////////////////////////////////////////

c_count_files_dir::c_count_files_dir(CRfmClientSocket *pTransport) :
	c_LogicCommand(pTransport)
{
	m_i_count_files = 0;
}

c_count_files_dir::~c_count_files_dir(void)
{
}



// ���������� ��������� ������ � ��������� �������� 
// sz_path_server - ���� � ��������

UINT c_count_files_dir::cmd_count_files_dir(CString sz_path_server)
{
	::ResetEvent(m_h_end_operation);
	m_sz_error_descr.Empty();
	m_flag_finish = false;
	m_i_count_files = 0;

	if(!m_pTransport->is_connected())
		return false;
	
	// �������� ������� ������� 

	char *p_packet = NULL;
	int size_pack = ((c_TransportLevel*)m_pTransport)->make_packet(ID_COUNT_FILES_DIR, 0, 0L, 
		sz_path_server.GetLength()+1, 
		sz_path_server.GetBuffer(), &p_packet);	
	((c_TransportLevel*)m_pTransport)->SendData(p_packet, size_pack);
	if(p_packet) delete [] p_packet;
	// ������� ������� ��������� ������

	DWORD i_ret = ::WaitForSingleObject(m_h_end_operation, MAX_TIME_WAIT);
	if(i_ret == WAIT_TIMEOUT)
	{
		//sz_err_descr = "��������� ����� �������� ������ � �������!";
		return false;
	}
	
	return m_i_count_files;
}

// ����� ������� �������� � ������ ���������� ����� � ��������
// ��� ������ ������

void c_count_files_dir::break_operation()
{
	m_sz_error_descr = "��� ����� � ��������!";
	m_flag_finish = false;
	::SetEvent(m_h_end_operation);
}

// �������� ������ ��� ���������� �������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_count_files_dir::ProcessCommand(char *pData, int nSize)
{
	if(!pData || !nSize)
		return;
	
	int i_size=0;
	char *p_data_ret = NULL, *p_data_packet=NULL;

	PPACKET_DATA pPacket = (PPACKET_DATA)pData; 
	char *pdata_pack = pData + sizeof(PACKET_DATA); 
	if(pPacket->c_code_err != 0)
		return;
	
	// ��������� ��������
	
	m_i_count_files = (UINT)pPacket->ui_total_size;
	::SetEvent(m_h_end_operation);	
}



















