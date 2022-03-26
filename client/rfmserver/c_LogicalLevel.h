#pragma once
#include "c_CommandHanlers.h"
#include <map>
using namespace std;


typedef map<unsigned char, c_LogicCommand*> CArrHandlersCmds; 


/////////////////////////////////////////////////////////////////////////////////////
// �����	: c_LogicalLevel
// ��������	: ������ ����� �������� �� ���������� ���������� �������
/////////////////////////////////////////////////////////////////////////////////////

class c_LogicalLevel
{
public:
	c_LogicalLevel(CRfmServerClient *pTransportLevel);
	~c_LogicalLevel(void);
	// ���������� ����� �� ���������� ������ 
	// pData - ������ ��� ����������� ������
	// iSize - ������ ������
	bool ProcessData(char *pData, int iSize);
	// �������� ����� ���������� �������
	// id_command - ID ������� � �������� �������� ����������
	// pCommand - ������ ��������� �������
	void AddNewHandler(unsigned char id_command, c_LogicCommand *pCommand);
	// ������� ������� 
	// id_command - ID ������� � �������� �������� ����������
	void DeleteHandler(unsigned char id_command);
	// ���������� ���������� ������� �� �� ID
	// id_command - ID ������� � �������� �������� ����������
	c_LogicCommand *GetHandlerByID(unsigned char id_command);
	// ���� ����� ������� ������� ��� ���������� � �������� �������
	void set_break_operation();
private:
	// ��������� ������������ ������
	CArrHandlersCmds m_arrHandlersCmds;
};
