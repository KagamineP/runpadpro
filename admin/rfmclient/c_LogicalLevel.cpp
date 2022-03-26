#include "StdAfx.h"
#include "c_LogicalLevel.h"



c_LogicalLevel::c_LogicalLevel(CRfmClientSocket *pTransportLevel)
{
	if(!pTransportLevel) 
		throw;

	c_LogicCommand *pCommand = NULL;
	
	// ��������� ������������� ������

	pCommand = new c_get_list_drive_cmd(pTransportLevel);
	AddNewHandler(ID_GET_LIST_DRIVES, pCommand);
	pCommand = new c_get_list_files_cmd(pTransportLevel);
	AddNewHandler(ID_GET_LIST_FILES, pCommand);
	pCommand = new c_delete_file_cmd(pTransportLevel);
	AddNewHandler(ID_DELETE_FILE, pCommand);
	pCommand = new c_create_dir_cmd(pTransportLevel);
	AddNewHandler(ID_CREATE_DIR, pCommand);
	pCommand = new c_copy_file_in(pTransportLevel);
	AddNewHandler(ID_COPY_IN, pCommand);
	pCommand = new c_copy_file_out(pTransportLevel);
	AddNewHandler(ID_COPY_OUT, pCommand);
	pCommand = new c_move_file_in(pTransportLevel);
	AddNewHandler(ID_MOVE_IN, pCommand);
	pCommand = new c_move_file_out(pTransportLevel);
	AddNewHandler(ID_MOVE_OUT, pCommand);
	pCommand = new c_rename_file_cmd(pTransportLevel);
	AddNewHandler(ID_RENAME_FILE, pCommand);
	pCommand = new c_exec_file_cmd(pTransportLevel);
	AddNewHandler(ID_EXEC_FILE, pCommand);
	pCommand = new c_count_files_dir(pTransportLevel);
	AddNewHandler(ID_COUNT_FILES_DIR, pCommand);
}

c_LogicalLevel::~c_LogicalLevel(void)
{
	for(CArrHandlersCmds::iterator iter = m_arrHandlersCmds.begin(); 
		iter != m_arrHandlersCmds.end(); iter++)
		if((*iter).second)
			delete (*iter).second;
	
	m_arrHandlersCmds.clear();
}

// ���� ����� ������� ������� ��� ���������� � �������� �������

void c_LogicalLevel::set_break_operation()
{
	// ���� ���� �������� ����� ��� ���������� �������
	
	for(CArrHandlersCmds::iterator iter = m_arrHandlersCmds.begin(); iter != m_arrHandlersCmds.end(); iter++)
		(*iter).second->break_operation();
			
}


// ���������� ����� �� ���������� ������ 
// pData - ������ ��� ����������� ������
// iSize - ������ ������

bool c_LogicalLevel::ProcessData(char *pData, int iSize)
{
	if(!pData || !iSize)
		return false;
	
	PPACKET_DATA pPacket = (PPACKET_DATA)pData;
	
	// ���������� ���������� �������

	c_LogicCommand *pLogicCommand = GetHandlerByID(pPacket->c_id_packet);
	if(!pLogicCommand) return false;

	// �������� ������ �� �������� ����������� �����������		

	pLogicCommand->ProcessCommand(pData, iSize);

	return true;
}

// �������� ����� ���������� �������
// id_command - ID ������� � �������� �������� ����������
// pCommand - ������ ��������� �������

void c_LogicalLevel::AddNewHandler(unsigned char id_command, c_LogicCommand *pCommand)
{
	if(!pCommand) throw;
	typedef pair <unsigned char, c_LogicCommand*> Mgr_Pair;
	m_arrHandlersCmds.insert(Mgr_Pair(id_command, pCommand));
}


// ������� ������� 
// id_command - ID ������� � �������� �������� ����������

void c_LogicalLevel::DeleteHandler(unsigned char id_command)
{
	CArrHandlersCmds::iterator iter = m_arrHandlersCmds.find(id_command); 
	if(iter == m_arrHandlersCmds.end())
		return;
	
	if((*iter).second)
		delete (*iter).second;
	
	m_arrHandlersCmds.erase(iter);
}

// ���������� ���������� ������� �� �� ID
// id_command - ID ������� � �������� �������� ����������

c_LogicCommand *c_LogicalLevel::GetHandlerByID(unsigned char id_command)
{
	
	
	CArrHandlersCmds::iterator iter = m_arrHandlersCmds.find(id_command); 
	if(iter == m_arrHandlersCmds.end())
		return NULL;
	return (*iter).second;
}