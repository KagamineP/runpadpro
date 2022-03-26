#pragma once

#include <deque>
using namespace std;

#include "RfmThreads.h"

class CRfmThreads;

typedef vector<CString> array_path; 


class CDisableWOW64FSRedirection
{
          void *old_value;
  
  public:
          CDisableWOW64FSRedirection();
          ~CDisableWOW64FSRedirection();
};



///////////////////////////////////////////////////////////////////////
// �������� ������ ������ �����
///////////////////////////////////////////////////////////////////////

typedef struct __DATA_FILE__
{
	CString		szNameFile;	// ��� �����
	BOOL		bDirectory;	// �������� �� ���� �����������
	BOOL		bDots;		// �������� �� ���������� ���������
	ULONGLONG	iSizeFile;	// ������ ����� � ������
	CTime		timeCreate;	// ���� �������� �����
	int			nIconFile;	// ������ ������ � ��������� ������ 
}DATA_FILE, *PDATA_FILE;

// ���������� ������ � ������ �� ���������� ����

typedef deque<PDATA_FILE> CListFiles;


///////////////////////////////////////////////////////////////////////
// �������� ������ ������ ��������� ����� � �������
///////////////////////////////////////////////////////////////////////

typedef struct __DATA_DISC__
{
	CString		szLetterDisc;	// ��������  ����������� �����
	UINT		iTypeDisc;		// ��� �����
	CString		szLabel;		// ����� ����;
	CString		szFileSys;		// ��� �������� �������
	int			nIconDisc;		// ������ ��� ���������� �����
	double		d_free_space;	// ��������� ����� �� �����
}DATA_DISC, *PDATA_DISC;

// ���������� ������ � ������

typedef deque<PDATA_DISC> CListDrives;

///////////////////////////////////////////////////////////////////////
// �����		: CNRFileData
// ��������		: ������������ ��� ������ � �������, � �� ������� 
//				  �� ��������� ����������	
///////////////////////////////////////////////////////////////////////

class CNRFileData
{
public:
	CNRFileData(CString szMainPath);
	virtual ~CNRFileData(void);
	// �������� ������ �� ��������� ������
	virtual bool GetDataFiles(CListFiles& files_list, CRfmThreads *p_proc_thread); 
	// ���������� �������� �� ���� ������������
	BOOL IsFileDirectory();
	// ��������� ������������� ����
	BOOL IsCurrentPathValid();
	// ���������� �������� �� ���� ����������
	// ����������� ([.] or [..])
	BOOL IsFileDots();
	// �������� �� ���� ������ ������
	BOOL IsFileNormal();
	// ���������� ������ ������ ������
	void GetImageListFiles();
	// ���������� ������ ������ ������
	static void GetImageListFiles(CImageList& imageFileList);
	// ���������� ������ ������ � �������
	// discList - ������ ���������� � ����� ������ ����������
	static void GetDiscList(CListDrives& discList);
	// ���������� ������ ���� � �����
	static CString GetFullPath(CString strCatalog, CString strFile, bool& b_prev_dir);
	// ������� ���� ��� �������
	static BOOL DeleteFiles(CString str_path, CRfmThreads *p_proc_thread);
	// ���������� ������ �������� ��� �����
	static ULONGLONG GetFileSizeOrDir(CString str_path, HWND& hWndProcess, HANDLE hEventStop); 
	// ���������� ���������� ������ � ������
	UINT get_count_files(array_path& arr_path_files); 
private:
	// ���������� ���������� ������ � ��������
	UINT get_count_files_of_dir(CString path_dir); 
	// ���������� ������ ��������
	static ULONGLONG GetSizeDir(CString str_path, HWND& hWndProcess, HANDLE hEventStop);
	// ������� ������� ����������
	static BOOL RemoveDir(CString strPath, CRfmThreads *p_proc_thread);
	// ������������� ����
	//CListFiles	m_listFiles;
	// ������� ���� � �������� � �������� ������� ��� �����
	CString m_szCurrentPath;
};
