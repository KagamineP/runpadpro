
#include <windows.h>


const WCHAR *text = L"������ � Bluetooth/IrDA �������������� �� ���������� ������������";
const WCHAR *caption = L"������ � Bluetooth/IrDA";


int WINAPI WinMain(HINSTANCE hThisInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
  MessageBoxW(NULL,text,caption,MB_OK | MB_ICONINFORMATION);
  ExitProcess(1);
}
