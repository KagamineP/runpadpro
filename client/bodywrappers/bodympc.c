
#include <windows.h>


const WCHAR *text = L"��� ������� ����������� Media Player Classic ������ ���������� ����������� Media Player Classic";
const WCHAR *caption = L"���������� Media Player Classic";


int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
  MessageBoxW(NULL,text,caption,MB_OK | MB_ICONINFORMATION);
  ExitProcess(1);
}
