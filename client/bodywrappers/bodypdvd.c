
#include <windows.h>


const WCHAR *text = L"��� ������� ����������� Power DVD ������ ���������� ����������� Power DVD";
const WCHAR *caption = L"���������� Power DVD";


int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
  MessageBoxW(NULL,text,caption,MB_OK | MB_ICONINFORMATION);
  ExitProcess(1);
}
