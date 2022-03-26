#pragma once
#include "commctrl.h"
#include "shellapi.h"
#include "RfmServerClient.h"
#include <deque>
using namespace std;

///////////////////////////////////////////////////////////////////////
// �������� ������ ������ �����
///////////////////////////////////////////////////////////////////////


class CDisableWOW64FSRedirection
{
          void *old_value;
  
  public:
          CDisableWOW64FSRedirection();
          ~CDisableWOW64FSRedirection();
};


typedef struct __DATA_FILE__
{
	std::string			szNameFile;				// ��� �����
	bool				bDirectory;				// �������� �� ���� �����������
	bool				bDots;					// �������� �� ���������� ���������
	unsigned __int64	iSizeFile;				// ������ ����� � ������
	FILETIME			timeCreate;				// ���� �������� �����
}DATA_FILE, *PDATA_FILE;

// ���������� ������ � ������ �� ���������� ����

typedef deque<PDATA_FILE> CListFiles;


///////////////////////////////////////////////////////////////////////
// �������� ������ ������ ��������� ����� � �������
///////////////////////////////////////////////////////////////////////

typedef struct __DATA_DISC__
{
	std::string		szLetterDisc;			// ��������  ����������� �����
	unsigned int	iTypeDisc;				// ��� �����
	std::string		szLabel;				// ����� ����;
	std::string 	szFileSys;				// ��� �������� �������
	double			d_free_space;			// ��������� ����� �� �����
}DATA_DISC, *PDATA_DISC;

// ���������� ������ � ������

typedef deque<PDATA_DISC> CListDrives;

///////////////////////////////////////////////////////////////////////
// �����		: CNRFileData
// ��������		: ������������ ��� ������ � �������, � �� �������
///////////////////////////////////////////////////////////////////////

class CNRFileData
{
public:
	CNRFileData(const char *szMainPath);
	~CNRFileData(void);
	// �������� ������ �� ��������� ������
	bool GetDataFiles(CListFiles& files_list); 
	// ���������� ������ ���� � �����
	static std::string GetFullPath(const char *strCatalog, const char *strFile);
	// ���������� �������� �� ���� ������������
	bool IsFileDirectory();
	// ������� ��������� ���� ��� ������ �������
	bool del_specif_file();
	// ������� �������
	static bool CreateDir(const char *str_path);
	// ���������� ������ ������ � �������
	// discList - ������ ���������� � ����� ������ ����������
	static void GetDiscList(CListDrives& discList);
	// ���������� ���������� ������ � ��������
	static UINT get_count_files_of_dir(std::string path_dir);
	// ��������� ������������� ����
	//bool IsCurrentPathValid();
	// ���������� �������� �� ���� ����������
	// ����������� ([.] or [..])
	//bool IsFileDots();
	// �������� �� ���� ������ ������
	//bool IsFileNormal();
	// ���������� ������ ������ ������
	//void GetImageListFiles();
	// ���������� ������ ������ ������
	//static void GetImageListFiles(HIMAGELIST& imageFileList);
	// ������� ���� ��� �������
	//static bool DeleteFiles(const char *str_path, HANDLE hThreadStop, CRfmServerClient *pConnCli);
	// ���������� ������ �������� ��� �����
	//static ULONGLONG GetFileSizeOrDir(const char *str_path, HANDLE hEventStop, CRfmServerClient *pConnCli);
	// ���������� ���������� ������ � ��������
	//static DWORD GetCountFilesDir(const char *str_path, HANDLE hEventStop, CRfmServerClient *pConnCli);
private:
	// ���������� ������ ��������
	//static ULONGLONG GetSizeDir(const char *str_path, HANDLE hEventStop, CRfmServerClient *pConnCli);
	// ������� ������� ����������
	//static bool RemoveDir(const char *strPath, HANDLE hThreadStop, CRfmServerClient *pConnCli);
	// ������������� ����
	//CListFiles	m_listFiles;
	// ������� ���� � �������� � �������� ������� ��� �����
	const char *m_szCurrentPath;
};
