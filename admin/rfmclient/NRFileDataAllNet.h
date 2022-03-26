#pragma once
#include "nrfiledatanet.h"

///////////////////////////////////////////////////////////////////////
// �����		: CNRFileDataAllNe
// ��������		: ������������ ��� ������ � �������, � �� ������� 
//				  �� �������. ������ ����� �������� �������� � ������� �� 
//				  ���� ������� ������������
///////////////////////////////////////////////////////////////////////

typedef vector<CRfmClientSocket*> CRfmSocketArr; 

class CNRFileDataAllNet : public CNRFileDataNet
{
public:
	CNRFileDataAllNet(CRfmSocketArr *p_arr_all_connect);
	~CNRFileDataAllNet(void);
	// �������� ������ �� ��������� ������ �� ���� �����������
	// � ������� ������ ���������� ������
	bool GetDataFiles(CListFiles& files_list, CRfmThreads *p_proc_thread); 
	// ���������� ����� ������ ������ �� ��������
	// discList - ������ ���������� � ����� ������ ����������
	bool GetDiscList(CListDrives& discList, CString& sz_err_descr);
private:
	// ������ �������� ����������� � ��������
	CRfmSocketArr *m_p_arr_all_connect;
	// �������� ���������� �����
	void SelectSameFile(CListFiles& files_list, CListFiles& file_list_new);
	// �������� ���������� �����
	void SelectSameDisc(CListDrives& discList, CListDrives& discList_new);
};
