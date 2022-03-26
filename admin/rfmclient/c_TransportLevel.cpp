#include "StdAfx.h"
#include "c_TransportLevel.h"

#pragma warning(disable : 4355)

// ����������� ������ ������

#define MIN_SIZE_PACKET 0x11
// ������������ ����� �������� ������ � �������
#define MAX_TIME_WAIT 15000

// ������� ���������� ����� � �������� 
UINT WM_DISCONNECT_SERVER = RegisterWindowMessage("message disconnect server");

c_TransportLevel::c_TransportLevel(HWND hwnd_tab, int i_indx_tab) : 
	m_LogicLevel(static_cast<CRfmClientSocket*>(this))
{
	memset(&m_prevPack, 0, sizeof(ORDER_DATA));
	m_iCountSend = 0;	
	m_iCountErrPack = 0;
	m_pch_cicle_bufer = new unsigned char[MAX_SIZE_CICLE_BUFER];
	m_i_point_write = m_i_point_read = m_i_need_count =0;
	m_i_tm_recv_pack = m_i_tm_send_pack = 0;
	m_b_send_packet = false;
	m_i_indx_tab = i_indx_tab;
	m_hwnd_tab = hwnd_tab;
	m_h_stop_current_process = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	::SetEvent(m_h_stop_current_process);
}

c_TransportLevel::~c_TransportLevel(void)
{
	if(m_prevPack.pData && m_prevPack.iSize)
		delete [] m_prevPack.pData;
	delete [] m_pch_cicle_bufer;
	::CloseHandle(m_h_stop_current_process);
}


// ���������� �������� �� ������ ������������� ������
// c_code_err - ��� ������

bool c_TransportLevel::IsTransportError(unsigned char c_code_err)
{
	switch(c_code_err)
	{
		case ERR_SIGN_PACK: return true;
		case ERR_ID_PACK:	return true;
		case ERR_SIZE_PACK: return true;
		case ERR_SRC_PACK:	return true;
	}
	return false;
}

// ���������� �������� �� ���������� ������������� ������
// c_id_packet - ���������� ������������� ������

bool c_TransportLevel::IsValidIDpacket(unsigned char c_id_packet)
{
	switch(c_id_packet)
	{	
		case  ID_GET_LIST_DRIVES:	return true;
		case  ID_GET_LIST_FILES:	return true;
		case  ID_DELETE_FILE:		return true;
		case  ID_CREATE_DIR:		return true;
		case  ID_COPY_IN:			return true;
		case  ID_COPY_OUT:			return true;
		case  ID_MOVE_IN:			return true;
		case  ID_MOVE_OUT:			return true;
		case  ID_RENAME_FILE:		return true;
		case  ID_EXEC_FILE:			return true;
		case  ID_ERROR_PACK:		return true;
		case  ID_COUNT_FILES_DIR:	return true;
	}
	return false;
}

// ��������� ����������� ����� ������
// pData - ������ ������
// nSize - ������ ������

unsigned short c_TransportLevel::CalcSRCpacket(char *pData, int nSize)
{
	unsigned short iSRCdata = 0;

	for(int i = 0; i < nSize; i++)
		iSRCdata += *pData++;

	iSRCdata ^= 0xFFFF;
	++iSRCdata;

	return iSRCdata;
}


// ��������� ����������� ����� ������
// pData - ������ ������
// nSize - ������ ������

bool c_TransportLevel::CheckSRCpacket(char *pData, int nSize)
{
	unsigned short i_src_pac = 0;
	
	if(!pData || nSize < sizeof(unsigned short))
		return false;
	
	// ��������� ����������� �����

	unsigned short i_calc_src = CalcSRCpacket(pData, nSize-sizeof(unsigned short));

	// �������� � ������ ����������� �����

	memcpy(&i_src_pac, pData+(nSize-sizeof(unsigned short)), sizeof(unsigned short));
	
	return (i_src_pac == i_calc_src);
}


// �������� ����� ������ ��� ����������� ������
// pData - ����������� �� ������ ������
// nSize - ������ ������

void c_TransportLevel::AddData(char *pData, int nSize)
{
	// ��������� ���� ��������� ������ � �������
		
	m_i_tm_recv_pack = GetTickCount();
	m_b_send_packet = false;
		
	if(!pData || !nSize)
		return;
	
	//ParsePacket(pData, nSize);			
	//return; 

	int i_total_size = 0;
	int i_size_pack = 0;
	char ch_pack_parse[MAX_SIZE_PACK];

	// ��������� ������ � ��������� �����
	
	if(nSize < (MAX_SIZE_CICLE_BUFER - m_i_point_write))
	{
		memcpy(m_pch_cicle_bufer + m_i_point_write, pData, nSize);
		
		int tst = m_i_point_write;
		
		m_i_point_write += nSize;
	
		if(m_i_point_write == MAX_SIZE_CICLE_BUFER)
			int s = 0;
	}
	else
	{
		memcpy(m_pch_cicle_bufer + m_i_point_write, pData, (MAX_SIZE_CICLE_BUFER - m_i_point_write));
		int delta = nSize - (MAX_SIZE_CICLE_BUFER - m_i_point_write);
		memcpy(m_pch_cicle_bufer, pData + (MAX_SIZE_CICLE_BUFER - m_i_point_write), delta);
		m_i_point_write = delta;

		if(m_i_point_write == MAX_SIZE_CICLE_BUFER)
			int s = 0;
	}
	
		
	while(m_i_point_write != m_i_point_read)
	{
		// ��������� ���� ������
		
		if(m_pch_cicle_bufer[m_i_point_read] != SIGN_BEGIN_PACK)
		{
			// ��������� ������ �������
			SendErrorMsg(ERR_SIGN_PACK);
			m_i_point_read = m_i_point_write;
			return;  
		}
		
		// ����� ������ ������ � ������

		if(m_i_point_write > m_i_point_read)
			i_total_size = m_i_point_write - m_i_point_read;
		else
			i_total_size = (m_i_point_write + MAX_SIZE_CICLE_BUFER) - m_i_point_read;

		// �������� ����� ������ ������
		
		i_size_pack = m_pch_cicle_bufer[((m_i_point_read + 1) >= MAX_SIZE_CICLE_BUFER) ? ((m_i_point_read + 1) - MAX_SIZE_CICLE_BUFER) : (m_i_point_read + 1)];
		i_size_pack |= (m_pch_cicle_bufer[((m_i_point_read + 2) >= MAX_SIZE_CICLE_BUFER) ? ((m_i_point_read + 2) - MAX_SIZE_CICLE_BUFER) : (m_i_point_read + 2)] << 8);

		// ��������� �� ���� �� ���������� ����� �� ������ �����������
		
		if(i_total_size >= i_size_pack)
		{
			if(i_size_pack <= (MAX_SIZE_CICLE_BUFER - m_i_point_read))
				memcpy(ch_pack_parse, m_pch_cicle_bufer + m_i_point_read, i_size_pack);
			else
			{
				memcpy(ch_pack_parse, m_pch_cicle_bufer + m_i_point_read, (MAX_SIZE_CICLE_BUFER - m_i_point_read));
				int delta = i_size_pack - (MAX_SIZE_CICLE_BUFER - m_i_point_read);
				memcpy(ch_pack_parse + (MAX_SIZE_CICLE_BUFER - m_i_point_read), m_pch_cicle_bufer, delta);
			}
						
			// �������� ����� �� ������ ����������

			ParsePacket(ch_pack_parse, i_size_pack);		
		
			// ����������� ��������� ������

			m_i_point_read += i_size_pack;
			if(m_i_point_read >= MAX_SIZE_CICLE_BUFER)
				m_i_point_read -= MAX_SIZE_CICLE_BUFER;

		}
		else
			return;
	}
}

// ����������� ������� ������
// pData - ������ �������� �����
// nSize - ������ ������

void c_TransportLevel::ParsePacket(char *pData, int nSize)
{
	if(!pData || !nSize)
		return;

	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData;
	
	
	// ��������� ����������� ����� ������

	if(!CheckSRCpacket(pData, nSize))
	{
		// ��������� ������ ������� � �����
		// ID_ERROR_PACK
		// ERR_SRC_PACK
		SendErrorMsg(ERR_SRC_PACK);
		return; 
	}
	
	
	// ��������� ��� ������
	
	if(!IsValidIDpacket(pPacket->c_id_packet))
	{
		// ��������� ������ ������� � �����
		// ID_ERROR_PACK
		// ERR_ID_PACK
		SendErrorMsg(ERR_ID_PACK);
		return; 
	}
	
	// ��������� ������ �� ������������ ������
	
	if(pPacket->c_id_packet == ID_ERROR_PACK)
	{
		if(IsTransportError(pPacket->c_code_err))
		{
			// ���������� ������ ������������� ������
			
			ProcessError();
			
			return;
		}
	}

	// ��������� ������ ������

	if((nSize-MIN_SIZE_PACKET) != pPacket->i_size_data)
	{
		// ��������� ������ ������� � �����
		// ID_ERROR_PACK
		// ERR_SIZE_PACK
		SendErrorMsg(ERR_SIZE_PACK);
		return; 
	}

	

	// �������� ������� �� ������ ������ � �������� ������� ������
	
	if(m_prevPack.pData && m_prevPack.iSize)
		delete [] m_prevPack.pData;
	m_prevPack.iSize = 0;
	m_iCountSend = 0;
	m_iCountErrPack = 0;

	
	// ����� ��������� ����������, �������� �� ���������� �������! 

	m_LogicLevel.ProcessData(pData, nSize);
}



// ��������� ����� � ����������� ������
// pData - ������ ������� ���������� �������� �������
// nSize - ������ ������

void c_TransportLevel::SendData(char *pData, int nSize)
{
	// ��������� ���� �������� ������
	
	m_i_tm_send_pack = GetTickCount();
	m_b_send_packet = true;

	// ����������� ����� ������ �� ������ ��������� ��������

	m_prevPack.pData = new char[nSize]; 
	m_prevPack.iSize = nSize;
	
	// �������� �����

	Send(pData, nSize);
}

// ��������� ������ �� �������� � ����

int c_TransportLevel::make_packet(unsigned char c_id_packet, unsigned char	c_code_err, 
								  unsigned __int64 ui_total_size, unsigned short	i_size_data, 
								  char *pData, char **pp_packet)
{
	char *p_packet = NULL;
	
	int total_size = sizeof(unsigned short) + sizeof(unsigned char) * 3 + sizeof(unsigned __int64) + 
		sizeof(unsigned short) + i_size_data + 2;
	
	


	*pp_packet = p_packet = new char[total_size];
	*p_packet++ = SIGN_BEGIN_PACK;	
	memcpy(p_packet, &total_size, sizeof(unsigned short));
	p_packet += sizeof(unsigned short);
	*p_packet++ = c_id_packet;
	*p_packet++ = c_code_err;
	memcpy(p_packet, &ui_total_size, sizeof(unsigned __int64));
	p_packet += sizeof(unsigned __int64);
	memcpy(p_packet, &i_size_data, sizeof(unsigned short));
	p_packet += sizeof(unsigned short);
	if(i_size_data)
	{
		memcpy(p_packet, pData, i_size_data);
		p_packet += i_size_data;	
	}
	
	// ��������� ����������� ����� ������
	
	unsigned short i_src = CalcSRCpacket(*pp_packet, total_size - sizeof(unsigned short));
	memcpy(p_packet, &i_src, sizeof(unsigned short));
	
	return total_size;
}


// ���������� ������ ������������� ������
// �������� �������� �����

void c_TransportLevel::ProcessError()
{
	// �������� ����� ��������

	Send(m_prevPack.pData, m_prevPack.iSize);
	m_iCountSend++;

	if(!m_prevPack.pData || !m_prevPack.iSize || m_iCountSend > COUNT_TRY_SEND)
	{
		// ��������� ���� ����� �������� �������� 	
		
		m_LogicLevel.set_break_operation();

		// ��������� ������� 
		//this->Disconnect(true);
		return;
	}
}

// �������� ����������� ������ ������� � �������� ��������� ������
// c_code_error - ��� ������ ������������� ������

void c_TransportLevel::SendErrorMsg(unsigned char c_code_error)
{
	m_i_tm_send_pack = GetTickCount();
	m_b_send_packet = true;

	if(m_iCountErrPack > COUNT_TRY_SEND)
	{
		// ��������� ���� ����� �������� �������� 	
		
		m_LogicLevel.set_break_operation();
		
		// ��������� �������
		//Disconnect(true);
		return;
	}
	
	char *p_packet = NULL;

	int i_size = make_packet(ID_ERROR_PACK, c_code_error, 0, 0, NULL, &p_packet);
	Send(p_packet, i_size);
	delete [] p_packet;
	
	// ��������� ������� �������� ��������� �������
	m_iCountErrPack++; 
}

// ��������� ���������� � ��������

void c_TransportLevel::OnCheckConnect()
{
	//if(!m_b_send_packet) return;
		
	//if(m_i_tm_recv_pack < m_i_tm_send_pack)
	//{
		if((GetTickCount() - m_i_tm_recv_pack) >  MAX_TIME_WAIT)
		{
			m_LogicLevel.set_break_operation();
			m_i_tm_recv_pack = GetTickCount();
		}
		//else
		//	m_i_tm_recv_pack = GetTickCount();

	//}

	//m_i_tm_recv_pack = m_i_tm_send_pack	= GetTickCount();
}

// ���������� ��� �������

void c_TransportLevel::set_name_server(CString sz_name_srv)
{
	m_sz_name_server = sz_name_srv;
}

// ������� ���������� ����� � ��������

void c_TransportLevel::OnDisconnect()
{
	//Disconnect();
	
	// ����������������� ��������� � ������� ������
	
	m_LogicLevel.set_break_operation();
		
	// ��������� ���� ���������� ������� ��������, ���� ��� ���������� 
	
	::WaitForSingleObject(m_h_stop_current_process, INFINITE);
	

	// �������� ��������� ���������� ���� ��� ������� ����� � ��������  
	
	::PostMessage(m_hwnd_tab, WM_DISCONNECT_SERVER, (WPARAM)m_i_indx_tab, 0x0);
}