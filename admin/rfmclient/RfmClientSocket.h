#pragma once
#include "NRFileData.h"


////////////////////////////////////////////////////////////
// ����� ���������� ������� ��� ������ ������� � �������� //
////////////////////////////////////////////////////////////

class CRfmClientSocket
{
public:
	CRfmClientSocket(void);
	virtual ~CRfmClientSocket(void);
	// ����������� � ������� 
	// pszServer - ����� �������
	// port - ���� ���������� � ��������
	bool Connect(const char* pszServer, int port);
	// ���������� �� �������
	bool Disconnect();
	// ����� ������ �������� ���������.
	// ��� ������� ����������� � CClientSocket ������ ����� ����� ������ ��������.
	// pInBuffer -  ��������� �� ������� �����
	// nBufSize  -  ������ �������� �����
	void HandleReceive(char* pInBuffer, size_t nBufSize);
	// ���������� ����� ���������� � ��������
	SOCKET get_client_socket() const {return m_listening_sock;}
	// ������� ������ ����� � �������� 
	virtual void OnDisconnect() = 0;
	// ���������� ���������� � ������ ����� � �������
	bool is_connected() const { return m_b_connected;}
	// ���������� ���� ����������� � �������
	void set_connected(bool b_connected){m_b_connected = b_connected;}
protected:
	// �������� ����� �������
	// pData - ������ �� ��������
	// nSize - ������ ������
	bool Send(char *pData, int nSize);
	// �������� ����� ������ ��� ����������� ������
	// pData - ����������� �� ������ ������
	// nSize - ������ ������
	virtual void AddData(char *pData, int nSize) = 0;
	// ���� ����������� � �������
	bool m_b_connected;
private:
	// ����� ���������� � ��������
	SOCKET m_listening_sock;
	// ID ������ �������� ������ � �������
	DWORD m_i_id_thread;
	// ���������� ������ �������� ������ � �������
	HANDLE m_h_thread;

};