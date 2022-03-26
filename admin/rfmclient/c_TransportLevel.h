#pragma once
#include ".\Rfmclientsocket.h"
#include "c_LogicalLevel.h"

#define SIGN_BEGIN_PACK		0x2A	// ������� ������ ������
#define COUNT_TRY_SEND		0xA		// ���������� ��� �������� ������, ����� �������� ����������
									// ���������� �������	
#define MAX_SIZE_CICLE_BUFER 102400 // ������������ ������ ��������� ���������� ������
#define MAX_SIZE_PACK 30720			// ������������ ������ ��������� ������




/////////////////////////////////////////////////////////////////////////////////////
// ��������� ������ ������������� ������  
/////////////////////////////////////////////////////////////////////////////////////

#define ERR_SIGN_PACK		0x50 // �� ������ ������� ������
#define ERR_ID_PACK			0x51 // �� ���������� ��� ������
#define ERR_SIZE_PACK	    0x52 // �� ���������� ������ ������
#define ERR_SRC_PACK	    0x53 // �� ���������� ����������� ����� ������


/////////////////////////////////////////////////////////////////////////////////////
// �������� ������� �� ������ ������ (�� ������� �������� ����� ��������)  
/////////////////////////////////////////////////////////////////////////////////////

typedef struct __ORDER_DATA__
{
	unsigned short iSize;			   // ������ ��������� ������
	char		   *pData;			   // ������ ����������� ������	
}ORDER_DATA;

/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_TransportLevel
// ��������	: ������ ����� �������� �� �������� ������ �� ������������ ������.  
/////////////////////////////////////////////////////////////////////////////////////

class c_TransportLevel : public CRfmClientSocket
{
public:
	c_TransportLevel(HWND hwnd_tab, int i_indx_tab);
	~c_TransportLevel(void);
	// ��������� ����� � ����������� ������
	// pData - ������ ������� ���������� �������� �������
	// nSize - ������ ������
	void SendData(char *pData, int nSize);
	// ��������� ������ �� �������� � ����
	int c_TransportLevel::make_packet(unsigned char c_id_packet, unsigned char	c_code_err, 
									  unsigned __int64 ui_total_size, unsigned short	i_size_data, 
									  char *pData, char **pp_packet);
	// �������� ��������� �� ���������� ������� ���������
	c_LogicalLevel *get_logic_level(){return &m_LogicLevel;}
	// ��������� ���������� � ��������
	void OnCheckConnect();
	// ���������� ��� �������
	void set_name_server(CString sz_name_srv);
	// ���������� ��� �������
	CString get_name_server(){return m_sz_name_server;}
	// ������� ���������� ����� � ��������
	void OnDisconnect();
	// ���������� ������������� ����������
	UINT get_id_connected() const {return m_i_indx_tab;}
	// ���������� ������� �������� ������� ��������
	HANDLE get_event_stop() const { return m_h_stop_current_process;}
private:
	// ������� �������� ������� �������� �� ����������
	// ������� ����� � ��������
	mutable HANDLE m_h_stop_current_process;
	// �������� ����� ������ ��� ����������� ������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	void AddData(char *pData, int nSize);
	// ����������� ������� ������
	// pData - ������ �������� �����
	// nSize - ������ ������
	void ParsePacket(char *pData, int nSize);
	// ���������� �������� �� ������ ������������� ������
	// c_code_err - ��� ������
	bool IsTransportError(unsigned char c_code_err);
	// ���������� �������� �� ���������� ������������� ������
	// c_id_packet - ���������� ������������� ������
	bool IsValidIDpacket(unsigned char c_id_packet);
	// ��������� ����������� ����� ������
	// pData - ������ ������
	// nSize - ������ ������
	bool CheckSRCpacket(char *pData, int nSize);
	// ��������� ����������� ����� ������
	// pData - ������ ������
	// nSize - ������ ������
	unsigned short CalcSRCpacket(char *pData, int nSize);
	// ���������� ������ ������������� ������
	// �������� �������� �����
	void ProcessError();
	// �������� ����������� ������ ������� � �������� ��������� ������
	// c_code_error - ��� ������ ������������� ������
	void SendErrorMsg(unsigned char c_code_error);
	// ����������� ���������� ����� �� ������ ��������� ��������
	ORDER_DATA m_prevPack;
	// ������� ������� ������ � ���� �� ������ �������
	short	   m_iCountSend;
	// ������� �������� ��������� ������� �� ����
	short	   m_iCountErrPack;	
	// ������ ����������� ������ ���������
	c_LogicalLevel m_LogicLevel;
	// ��������� �������� �����
	unsigned char *m_pch_cicle_bufer;
	// ��������� ������ ��������� ������
	int m_i_point_read;
	// ��������� ������ ��������� ������
	int m_i_point_write;
	// ���������� ���� ��������� ��� ������� ������
	int m_i_need_count;
	// ����� ��������� ������
	DWORD m_i_tm_recv_pack;
	// ����� �������� ������
	DWORD m_i_tm_send_pack;
	// ���� �������� ������
	bool m_b_send_packet;
	// �������� �������
	CString m_sz_name_server;
	// ����, ������������ ������� ����������
	HWND m_hwnd_tab;
	// ������������� ����, ������������� ������� ����������
	int m_i_indx_tab;
};
