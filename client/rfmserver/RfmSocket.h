#pragma once

#include <map>
using namespace std; 

class CRfmSocket;

////////////////////////////////////////////////////////////////////////
// �����	: CClients
// ��������	: ������� ����� ��� ������� ���������� ������������
//			  ���������				
////////////////////////////////////////////////////////////////////////


class CClients	
{
public:
	CClients(SOCKET sock_client, sockaddr_in addr_client, size_t i_len_addr, unsigned int id_client, CRfmSocket *p_rfm_socket)
	{
		m_p_rfm_socket = p_rfm_socket;
	};
	virtual ~CClients(void){}
	CRfmSocket *get_rfm_socket() {return m_p_rfm_socket;}
protected:	
	// ��������� �� ����� ����������� ��������
	CRfmSocket *m_p_rfm_socket;
};
	
typedef map<unsigned int, CClients*> c_arr_clients;

////////////////////////////////////////////////////////////////////////
// �����	: CRfmSocket
// ��������	: ������������� ������� ��� ������ � �������� �� ��������� 
//			  ������� 	
////////////////////////////////////////////////////////////////////////

class CRfmSocket
{
public:
	CRfmSocket(void);
	~CRfmSocket(void);
	// ����� ������� 
	// p_name_server - ��� �������
	// i_port - ���� �������
	bool StartServer(const char* p_name_server, unsigned short i_port);
	// ������� ������� 
	bool StopServer();
	// ���������� ����� ������������� �������
	SOCKET get_socket_listen() const {return m_listening_sock;}
	// ���������� ������� ���������� ��������
	HANDLE get_event_disconnect() const { return m_h_disconnect_client;}
	// ���������� ������������� ���������� �������
	unsigned int get_id_delete_client() const { return m_i_id_delete_cl; }
	// �������� ������ ������� 
	// sock_client - ����� ���������� � ��������
	// addr_client - ����� �������
	// i_len_addr - ����� ������
	void add_new_client(SOCKET sock_client, sockaddr_in addr_client, size_t i_len_addr);
	// ������� ������� 
	// p_clietnt - ��������� �� ������ ����������
	void delete_client(int i_id_client);
	// ������� ���� ��������
	void delete_clients_all();
	// ���������� ������� �������� �������
	void set_delete_client(unsigned int id_delete)
	{
		m_i_id_delete_cl = id_delete;	 
		::SetEvent(m_h_disconnect_client);
	}
	// ���������, �������� �� ��������� ������
	bool check_same_client(sockaddr_in addr_client, size_t i_len_addr);
private:
	// ����� ������� ��� �������������
	SOCKET m_listening_sock;
	// ������������� ������� � ���� ������������ ��������
	CRITICAL_SECTION m_cs_clients;
	// ��������� ������������ ��������
	c_arr_clients m_arr_clients;
	// ������������� ������
	DWORD m_i_id_thread;
	// ���������� ������ ����������� ��������
	HANDLE m_h_thread;
	// ���������� ������ ������������
	HANDLE m_h_thread_mtc;
	// ������������� ������ ������������
	DWORD m_i_id_thread_mtc;
	// ���������� ������������� �������
	// ���������������� ������ ��� ��� ���������� �������
	unsigned int m_i_id_guid;
	// ������� ���������� ������� �� �������
	HANDLE m_h_disconnect_client;
	// ID ���������� �������
	unsigned int m_i_id_delete_cl; 
};
