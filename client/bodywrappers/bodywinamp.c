
#include <windows.h>


const WCHAR *text = L"��� ������� ����������� Winamp ������ ���������� ����������� Winamp";
const WCHAR *caption = L"���������� Winamp";


int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
  MessageBoxW(NULL,text,caption,MB_OK | MB_ICONINFORMATION);
  ExitProcess(1);
}
