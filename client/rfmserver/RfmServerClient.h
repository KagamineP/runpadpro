#pragma once

#include "RfmSocket.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �����	: CRfmServerClient
// �������� : ������ ����� ������������� ������� ��� ������ � ������ ������������ ��������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CRfmServerClient : public CClients
{
public:
	CRfmServerClient(SOCKET sock_client, sockaddr_in addr_client, size_t i_len_addr, unsigned int id_client, CRfmSocket *p_rfm_socket);
	virtual ~CRfmServerClient(void);
	// ������� ���������� � ������� ��������
	void close_client();
	// ���������� ID �������
	unsigned int get_id_client() const { return m_id_client;}
	// ���������� ����� �������
	SOCKET get_client_socket() const {return m_sock_client;}
	// ���������� ����� �������
	sockaddr_in get_client_add() const {return m_addr_client;}
	/// ����� ������ �� �������.
	/// ��� ������ ����������� � CServerClientBase ������ ����� ����� ������ �� �������.
	/// pInBuffer -  ��������� �� ������� �����
	/// nBufSize  -  ������ �������� �����
	void OnReceive(char* pInBuffer, size_t nBufSize);
	// ������ ������������ ����� �������
	bool start_client();
	// ������� ���������� �������
	virtual void OnDisconnect() = 0;
protected:
	// �������� ������ � �����
	// pData - ������ �������������� �� ��������
	// nSize - ������ ������
	bool Send(char *pData, int nSize);
	// �������� ����� ������ ��� ����������� ������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void AddData(char *pData, int nSize) = 0;
	// ���������� ID �������
	unsigned int m_id_client;
	// ����� ������������� �������
	SOCKET m_sock_client;
private:
	// ����� ������������� �������
	sockaddr_in m_addr_client; 
	// ����� ������ ������������� �������
	size_t m_i_len_addr;
	// ����� ��������� ������ �������
	HANDLE m_h_thread_client;
	// ID ������ ��������� ������ ������� 
	DWORD m_i_id_thread;	
};
