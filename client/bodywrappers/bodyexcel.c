
#include <windows.h>


const WCHAR *text = L"��� ������� ����������� Excel ������ ���������� ����������� excel.exe";
const WCHAR *caption = L"���������� Excel";


int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
  MessageBoxW(NULL,text,caption,MB_OK | MB_ICONINFORMATION);
  ExitProcess(1);
}
