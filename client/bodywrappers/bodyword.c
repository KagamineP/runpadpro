
#include <windows.h>


const WCHAR *text = L"��� ������� ����������� Word ������ ���������� ����������� winword.exe";
const WCHAR *caption = L"���������� Word";


int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
  MessageBoxW(NULL,text,caption,MB_OK | MB_ICONINFORMATION);
  ExitProcess(1);
}
