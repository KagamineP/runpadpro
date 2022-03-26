unit lang;

interface

// todo: cleanup

const S_WARNING    = '��������������';
const S_QUESTION   = '������';
const S_ERR        = '������';
const S_INFO       = '���������';
const S_SELECTFOLDER = '����� �����';
const S_SELECTFILEFOLDER = '����� �����/�����';
const S_SELECTFILE = '����� �����';
const S_SELECTFOLDERREMOTE = '������� ������ ���� � �����:';
const S_SELECTFILEFOLDERREMOTE = '������� ������ ���� � �����/�����:';
const S_SELECTFILEREMOTE = '������� ������ ���� � �����';
const S_SELECTFOLDERLOCAL = '�������� �����:';
const S_SELECTFILEFOLDERLOCAL = '�������� �����/����:';
const S_SELECTFILELOCAL = '�������� ����';
const S_SLOTBUSY    = '�����';
const S_SLOTFREE    = '��������';
const S_QUERYTOFREE = '�������� ���� ����?';
const S_ERRSLOTBUSY = '���� ���� ��� �����';
const S_INSERTFLASH = '�������� flash-���� � ������� "��"';
const S_ERRFLASHNOTFOUND = '���� �� ������';
const S_ERRFLASH = '������ ��������� ���������� � �����. �������� ���� �� ������������';
const S_NEEDFLASHFORMAT = '���������� ��������������� flash-����!';
const S_FLASHOK = '������� ���������!';
const S_BLOCKWARN = '��������� ��� ����� ������ ��� ������������� API!';
const S_HOURS = '�����';
const S_DEFSHEETNAME = '����� ��������';
const S_DEFSHORTCUTNAME = '����� �����';
const S_CONFIRMDELETESHEET = '������� ��������?';
const S_EMPTYELEM = '���������� ��������/����� � ������ ������';
const S_NOTUNIQUEELEM = '���������� ��������/����� � ������������ ������';
const S_CONNECTED = '����������';
const S_NOTCONNECTED = '��� �����������';
const S_CONNECTING = '�����������...';
const S_ERRCONNECT = '������ �����������';
const S_CONFIRMDELETEPROFILE = '������� �������';
const S_PROFILEINUSE = '������ ������� �� ����� ���� ������, �.�. ������������ � ��������';
const S_PROFILESNOTSELECTED = '�� ������� ������� �������� �� ������ ���������';
const S_EMPTYPROFILENAME = '��� ������� �� ����� ���� ������';
const S_PROFILEEXISTS = '������� � ����� ������ ��� ����������';
const S_CONFIRMDELETEUSER = '������� ������������ �� ������ (�� �� SQL-����)';
const S_ADDNEWUSERTODB = '���������� ������ ������������ � ���� Runpad';
const S_CHANGEUSERDBPWD = '��������� ������ ������������ ��';
const S_DIFFERENTPWDS = '�� ����� ������ ������';
const S_EMPTYUSERNAME = '�� �� ����� ��� ������������';
const S_EMPTYUSERPWD = '�� �� ����� ������';
const S_ERR_CONNECT = '������ �����������/�����';
const S_ERR_ACCESS = '� ��� ��� ���� ��� �������, ���� �� ���� �������� ������� ��������� ��������� ��';
const S_USER_EXISTS = '������������ ��� ����������.'#13#10'�������� ��� �� ������ � ������� "���������� �����"';
const S_PWD_CHANGED = '������ ������������ �������!';
const S_ERR_PWD_CHANGE = '������������ �� ���������� ��� � ��� ��� ���� ��� ������������� ��������';
const S_ACCESS_SET = '����� �����������!';
const S_USER_ADDED = '������������ ��������!'#13#10'�� �������� ���������� ������ ��� �����!';
const S_USERNAME_INVCHARS = '��� �� ������ ��������� ����������� ��������, ���� ����������������� ��� ���������� � �����';
const S_TIP0 = '�� ���������� ���� � ����� ������������!'#13#10'������������� ����� ������������ ������������ ������������� ��� ��������� ������������ � �������� ������ �������������.'#13#10'���������� ���� ����� �� ������� "������ � �������".';
const S_TIP1 = '����� ������������ ����������� �� ����, �� ������� ���������� ������.'#13#10'���������� ���� ������� ����� ������������ ������� (�������������), ���� ��������� ������ �� ���� ����.';
const S_TIP2 = '�� �������� ��������� ���������� ����� ("Secure Mode") � ���������� D-Tools �� ���������� �������!';
const S_TIP3 = '����������� ������������ �� �������� �� ���� �������� ������������ ����, ��� �������� ���� � ������ � ���� ����������.';
const S_SELECTUSER = '�������� ��� �������� ������������ ��� ��������������';
const S_DISABLEPWD = '��������� ������?';
const S_SETPWD = '���������� ����� ������?';
const S_ENTERTHEMEURL = '<������� URL ��� ���� � HTML-�����>';
const S_CONTENTPROFILE = '������� ��������';
const S_ERR_FILEREAD = '������ ������ �� �����';
const S_ERR_FILEWRITE = '������ ������ � ����';
const S_QUERY_SORTSHORTCUTS = '������������� ������ ���� ��������?';

const S_RULESHELPUSER =
'"������� ��� ������� �������� (��� ������������)"'#13#10+
'---------'#13#10+
'���������� ������� ����� ���� ��������� ��� ������������ ��� ��� ����������, �.�. �� ����� ���������� ����� ���� ��������� �������������.'#13#10+
'�� ���� �������� ��������������� ���������, ����������� ������ � ������������.'#13#10+
'���������� �������� (����) ����������� �� ������������, � �� �� ����������!'#13#10+
'��� ��������� �������� ���������� ������ ���� ��� ��������� ������, �� ������� ����� ���������� ������� �������� ��� ����������� ����� ��� ��� ������.'#13#10+
'���� �� ���� ������� ������������ �������, � ������ ���������� �������� ������ �������� ��������������� ������� ������� � ���������� �� �� ���������� ������ �� ������� �� ����� (��� ���������� ��� ������ �����).'#13#10+
'������� ��������������� ���������������, ������ �� ������� ����� ��������. '#13#10+
'���� ������� ��� ������� �� ���������, �� ���������� ������� � ������ ���������� �� ������ �������, � ��� �����, ���� �� ����� ��������� �������. '+'���� �� ���� ������� �� ������� ����������, �� ���������� ���� ����� ����� ��������� �� ���������� ���� (��� ������������ ��������� �� ���������).'#13#10+
'������ ������� �������� ����� - ��� �������� ���������� ��������, ��������� ������� ������������ ����� ��������� ����� �� ���������� ������. '#13#10+
'��� �� ������:'#13#10+
'@computerloc - �������������� ����������'#13#10+
'@computerdesc - �������� ���������� (����� ���� ���������� ����� ������)'#13#10+
'@computername - NETBIOS ��� ����������'#13#10+
'@ip - IP-����� ���������� � ������� �.�.�.�'#13#10+
'@userdomain - ����� ������������'#13#10+
'@username - ��� ������������ Windows'#13#10+
'@langid - ��������� ���� ����� (0-���,1-����,2-���)'#13#10+
'� �������� ���������� ������� ��� ������� ����� ������������ �����������: =, <>, >, <, >=, <=.'#13#10+
'� �������� ������� �������� ��� ������� ����� ������������ �����������: NOT, OR, AND.'#13#10+
'����������� ������������ ������ ��� �������������. ������� �� ��������� �������� ������������.'#13#10+
'����������� ��������� �������� ������ ���� � ��������� ��������. ���� �� � ����� �������� ���� ��������� �������, �� ����� �� �������� ������ ������.'#13#10+
'���� � �������� ������� ������� ������ ������, ��� ����� �������� ����������� ���������� (�.�. ��� ����� ������� ���������� ����� ���������� �������), ������ ������ ����� ����� ��������� ����� ������� � ����� ����� ������ ������.'#13#10+
'���� � ��� ��� ���� ����� � ������������� ������ ���� ���������� ���������, �� ���������� �������� ���� ������� � ������ ������� �������.'#13#10+
'������ ������� "��� ������������ ''user'' �������� ��������������� �������":'#13#10+
'@username = ''user'''#13#10+
'������ ������� "��� ���� ������������� ����� ''student'' ��� ������ � IP 192.168.0.2 �������� ��������������� �������":'#13#10+
'(@username <> ''student'') OR (@ip = ''192.168.0.2'')'#13#10+
'����� ������� ����� ���������� � ����� ������� �������.'#13#10+
'����, ��������, � �������� ������ � ��� ���������� ������ �� ����� � ����� ���������� ��� ������ �� "������"/"������" � ������� (� �� ��������), �� ������� ��������� �� �����:'#13#10+
'@computerdesc > 25'#13#10;

const S_RULESHELPCOMP =
'"������� ��� ������� �������� (��� ����������)"'#13#10+
'---------'#13#10+
'�� ������� ������� ��� ���������� ����� �� ���������� �� ������ ��� ������������, ������ � ��� ������ ������������ ��������� ����������:'#13#10+
'@userdomain, @username, @langid.'#13#10+
'��. �������� ������ ��� ������������ ��� ����� ��������� ����������.'#13#10;



implementation

end.
