
Name "Runpad Pro ��������"
!include "..\common\version_nsis.inc"

OutFile "setup.exe"
BGGradient 000000 000000 FFFFFF
BGAlpha 160
InstallDir "$PROGRAMFILES\Runpad Pro Operator"
InstallDirRegKey HKLM "SOFTWARE\RunpadProOperator" "Install_Dir"
ComponentText "�������� ����������� ���������� ��� ���������"
DirText "�������� ���������� ��� �����������"
UninstallText "���� ������ ������ ���������� � ������ ����������"
;ShowInstDetails show
AutoCloseWindow true
InstallColors /windows
UninstallExeName "uninstall.exe"



Function .onInit
  IfNewOS OSOk
    MessageBox MB_OK|MB_ICONEXCLAMATION "Win95/98/ME/NT4 �� �������������� ����������"
    Abort
  OSOk:
  
  IfAdmin RightsOK
    MessageBox MB_OK|MB_ICONEXCLAMATION "��������� ���������� ����������� ��-��� ������� ������ ��������������"
    Abort
  RightsOK:

  FindWindow goto:Abort1 "_RSOperatorClass"
  goto NoAbort1
  Abort1:
    MessageBox MB_OK|MB_ICONSTOP '�� ������ ������� ��������� "Runpad Pro ��������" ����� ������������'
    Abort
  NoAbort1:

  GetACP $0
  StrCmp $0 "1251" _CYR
    MessageBox MB_OK|MB_ICONEXCLAMATION "� ����� ������������ ���������� ���� �� ��������� (��� ��-Unicode ��������) ������ �� �������������.$\n�������� ��� �� �������, ����������, �����������, ���������� ��� ���������, � ����� ���������� ���� ���������� ��������������/������.$\n� ��������� ������ ���������� ������ �� �� �������������."
  _CYR:

FunctionEnd


Function .onInstSuccess
 SetOutPath $INSTDIR
 Exec "$INSTDIR\rsoperator.exe -setup"
FunctionEnd


Function un.onInit
  IfAdmin RightsOK
    MessageBox MB_OK|MB_ICONEXCLAMATION "�������� ���������� ����������� ��-��� ������� ������ ��������������"
    Abort
  RightsOK:

  FindWindow goto:Abort1 "_RSOperatorClass"
  goto NoAbort1
  Abort1:
    MessageBox MB_OK|MB_ICONSTOP '�� ������ ������� ��������� "Runpad Pro ��������" ����� ���������'
    Abort
  NoAbort1:
FunctionEnd



Section ""
  SetOutPath $INSTDIR
  File "..\..\admin\sql\*.dll"

  WriteRegStr HKLM "SOFTWARE\RunpadProOperator" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RunpadProOperator" "DisplayName" "Runpad Pro ��������"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RunpadProOperator" "UninstallString" '"$INSTDIR\uninstall.exe"'

  CreateDirectory "$SMPROGRAMS_COMMON\Runpad Pro ��������"
  CreateShortCut "$SMPROGRAMS_COMMON\Runpad Pro ��������\�������� ���������.lnk" "$INSTDIR\uninstall.exe"
SectionEnd


Section "��������� ���������"
  
  SetOutPath $INSTDIR
  File "..\rsoperator\rsoperator.exe"
  File "..\rsoperator\dll\rsoperator.dll"
  File "..\rsstat\rsstat.exe"
  File "..\rsstat\dll\rsstat.dll"
  File "..\rsstat\help\rsstat.hlp"
  File "..\..\common\redist\gdiplus.dll"

  File "..\..\admin\rsrdclient\rsrdclient.exe"
  File "..\..\admin\rsrdclient\rsrdclient.dll"

  File "..\..\admin\rfmclient\rfmclient.exe"

  CreateShortCut "$SMPROGRAMS_COMMON\Runpad Pro ��������\��������� ���������.lnk" "$INSTDIR\rsoperator.exe"
  CreateShortCut "$SMPROGRAMS_COMMON\Runpad Pro ��������\��������� ���������.lnk" "$INSTDIR\rsoperator.exe" "-setup" "$INSTDIR\rsoperator.exe" 1
  CreateShortCut "$SMPROGRAMS_COMMON\Runpad Pro ��������\���������� ������� ��������.lnk" "$INSTDIR\rsstat.exe"
SectionEnd


Section "��������� ��������� �������"
  
  SetOutPath $INSTDIR
  File "..\..\admin\rsdbview\rsdbview.exe"

  CreateShortCut "$SMPROGRAMS_COMMON\Runpad Pro ��������\�������� �������.lnk" "$INSTDIR\rsdbview.exe"
SectionEnd



Section "Uninstall"
  call un.onInit

  RMDir /r "$SMPROGRAMS_COMMON\Runpad Pro ��������"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RunpadProOperator"
  DeleteRegValue HKLM "SOFTWARE\RunpadProOperator" "Install_Dir"

  RmDir /r "$INSTDIR"

SectionEnd

