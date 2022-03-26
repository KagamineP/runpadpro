unit startup;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ComCtrls, StdCtrls, ExtCtrls;


{$I ..\cfg.inc}

type
     TStartupInfoFull = record
      size : integer;
      small_icon : cardinal;
      big_icon : cardinal;
      machine_type:integer;
      machine_loc:TSTRING;
      machine_desc:TSTRING;
      server_ip:TSTRING;
      def_lang:integer;
      add_langs:longbool;
      show_langs_at_startup:longbool;
      allowed_services_ips:TSTRING;
     end;
     TStartupInfoShort = record
      size : integer;
      small_icon : cardinal;
      big_icon : cardinal;
      machine_type:integer;
      machine_loc:TSTRING;
      machine_desc:TSTRING;
      server_ip:TSTRING;
     end;
     PStartupInfoFull = ^TStartupInfoFull;
     PStartupInfoShort = ^TStartupInfoShort;


type
  TRSStartupForm = class(TForm)
    Panel1: TPanel;
    Image1: TImage;
    Bevel1: TBevel;
    Panel2: TPanel;
    Bevel2: TBevel;
    ButtonNext: TButton;
    ButtonPrev: TButton;
    ButtonCancel: TButton;
    Panel3: TPanel;
    PageControl: TPageControl;
    TabSheet1: TTabSheet;
    TabSheet2: TTabSheet;
    TabSheet3: TTabSheet;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    ComboBox1: TComboBox;
    Label4: TLabel;
    Edit1: TEdit;
    Label5: TLabel;
    Edit2: TEdit;
    Label6: TLabel;
    Edit3: TEdit;
    TabSheet4: TTabSheet;
    Memo1: TMemo;
    TabSheet5: TTabSheet;
    Label8: TLabel;
    Label7: TLabel;
    TabSheet6: TTabSheet;
    Label9: TLabel;
    ComboBox2: TComboBox;
    CheckBox1: TCheckBox;
    TabSheet7: TTabSheet;
    Label10: TLabel;
    Edit4: TEdit;
    procedure FormShow(Sender: TObject);
    procedure ButtonNextClick(Sender: TObject);
    procedure ButtonPrevClick(Sender: TObject);
    procedure TabSheet4Show(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure TabSheet3Show(Sender: TObject);
    procedure ComboBox2Change(Sender: TObject);
    procedure TabSheet7Show(Sender: TObject);
    procedure TabSheet2Show(Sender: TObject);
  private
    { Private declarations }
    info_f : PStartupInfoFull;
    info_s : PStartupInfoShort;
    is_short : boolean;
    can_close : boolean;
    procedure SwitchToPage(idx:integer);
    procedure UpdateButtons;
    function FromMachineType2Idx(machine_type:integer):integer;
    function FromIdx2MachineType(idx:integer):integer;
    function IsCyr:boolean;
  public
    { Public declarations }
    constructor CreateForm(p:pinteger);
    destructor Destroy(); override;
  end;


function ShowStartupMasterDialog(p:pinteger):longbool; cdecl;


implementation

{$R *.dfm}



function TRSStartupForm.IsCyr:boolean;
begin
 Result:=(GetACP()=1251);
end;


function TRSStartupForm.FromMachineType2Idx(machine_type:integer):integer;
begin
 Result:=0;
 case machine_type of
  MACHINE_TYPE_GAMECLUB:       Result:=0;
  MACHINE_TYPE_INETCAFE:       Result:=1;
  MACHINE_TYPE_OPERATOR:       Result:=2;
  MACHINE_TYPE_ORGANIZATION:   Result:=3;
  MACHINE_TYPE_TERMINAL:       Result:=4;
  MACHINE_TYPE_HOME:           Result:=5;
  MACHINE_TYPE_OTHER:          Result:=6;
 end;
end;

function TRSStartupForm.FromIdx2MachineType(idx:integer):integer;
begin
 Result:=MACHINE_TYPE_GAMECLUB;
 case idx of
  0: Result:=MACHINE_TYPE_GAMECLUB;
  1: Result:=MACHINE_TYPE_INETCAFE;
  2: Result:=MACHINE_TYPE_OPERATOR;
  3: Result:=MACHINE_TYPE_ORGANIZATION;
  4: Result:=MACHINE_TYPE_TERMINAL;
  5: Result:=MACHINE_TYPE_HOME;
  6: Result:=MACHINE_TYPE_OTHER;
 end;
end;

constructor TRSStartupForm.CreateForm(p:pinteger);
var n:integer;
begin
 inherited Create(nil);

 can_close:=false;

 is_short:=(p^=sizeof(TStartupInfoShort));
 info_f:=PStartupInfoFull(p);
 info_s:=PStartupInfoShort(p);

 Label1.Caption:='������� "�����" ��� ������� �������';
 Label3.Caption:='���������� ����� ����������:';
 Label7.Caption:='����������� �������� "������������ ������" ��� ������ ��������� � "�������� ���������" ��� ��������� ����������!';
 Label4.Caption:='�������������� ����������:';
 Label5.Caption:='�������� ���������� (����� ������ �����):';
 Label6.Caption:='IP-����� �������:';

 if not is_short then
  Label8.Caption:='������ ������� ���������!'#13#10#13#10'���� ������������� ��������� ��������, �� ����� ��������� ������������ ����������'#13#10+#13#10'������ ������ ����� ��������� �������� ��-��� ������� ������ �������������� ��� ��������� ��������'#13#10#13#10'�������� ���� ����� � ��������� ������ ����� ��������� ���������'#13#10#13#10'-------'#13#10'������� "���������" ��� ������'
 else
  Label8.Caption:='������ ������� ���������!'#13#10#13#10'���� ������������� ��������� ��������, �� ����� ��������� ������������ ����������'#13#10+#13#10'��� ���������� ������� ������� ���������� ����� ��������� ��������� ��������� ������� Runpad Pro'#13#10#13#10'��������� ���������� ������ ����� � ��������� ������ ����� ��������� ���������'#13#10#13#10'-------'#13#10'������� "���������" ��� ������';
 
 Label9.Caption:='���� �� ���������:';
 Label10.Caption:='����� ����� ������� ������ IP-������� ����� (����� ����� � �������), � ������� ����� ��������� ����������� � �������� ���������� ��������� ��������� � ���������� �������� ����� (������ ��� ������ ���������� � ��������������).'+' ���� ������ �� ���������, �� ����� �������� ������ � ����� ������.';

 if IsCyr then
  begin
   Caption:='������ ��������� Runpad Pro ������';
   ComboBox1.Items.Add('������ � ������� �����');
   ComboBox1.Items.Add('������ � ��������-����');
   ComboBox1.Items.Add('������������ ������');
   ComboBox1.Items.Add('������ � �����������/�����');
   ComboBox1.Items.Add('������������ ������');
   ComboBox1.Items.Add('�������� ���������');
   ComboBox1.Items.Add('������');
   ComboBox2.Items.Add('�������');
   ComboBox2.Items.Add('English');
   ComboBox2.Items.Add('���������');
   if (not is_short) and (info_f.add_langs) then
    begin
     ComboBox2.Items.Add('Deutsch');
     ComboBox2.Items.Add('Francais');
     ComboBox2.Items.Add('Romana');
     ComboBox2.Items.Add('E��nvika');
     ComboBox2.Items.Add('���������');
    end;
   CheckBox1.Caption:='���������� ���� ������ ����� ��� ������';
  end
 else
  begin
   Caption:='Settings master for Runpad Pro Client';
   ComboBox1.Items.Add('Machine in game-club');
   ComboBox1.Items.Add('Machine in internet-cafe');
   ComboBox1.Items.Add('Operator''s machine');
   ComboBox1.Items.Add('Machine in office');
   ComboBox1.Items.Add('Terminal server');
   ComboBox1.Items.Add('Home computer');
   ComboBox1.Items.Add('Other');
   ComboBox2.Items.Add('Russian');
   ComboBox2.Items.Add('English');
   ComboBox2.Items.Add('Ukrainian');
   if (not is_short) and (info_f.add_langs) then
    begin
     ComboBox2.Items.Add('Deutsch');
     ComboBox2.Items.Add('Francais');
     ComboBox2.Items.Add('Romana');
     ComboBox2.Items.Add('Greek');
     ComboBox2.Items.Add('Bulgarian');
    end;
   CheckBox1.Caption:='Show language selection at startup';
  end;

 ComboBox1.ItemIndex:=FromMachineType2Idx(info_s.machine_type);
 Edit1.Text:=info_s.machine_loc;
 Edit2.Text:=info_s.machine_desc;
 Edit3.Text:=info_s.server_ip;

 if not is_short then
  begin
   Edit4.Text:=info_f.allowed_services_ips;
   ComboBox2.ItemIndex:=info_f.def_lang;
   CheckBox1.Checked:=info_f.show_langs_at_startup;
  end;

 SwitchToPage(0);
 for n:=0 to PageControl.PageCount-1 do
  PageControl.Pages[n].TabVisible:=false;
 SwitchToPage(0);

 SendMessage(Handle, WM_SETICON, ICON_SMALL, info_s.small_icon);
 SendMessage(Handle, WM_SETICON, ICON_BIG, info_s.big_icon);
end;

destructor TRSStartupForm.Destroy();
begin
 if ModalResult=mrOk then
  begin
   info_s.machine_type:=FromIdx2MachineType(ComboBox1.ItemIndex);
   StrCopy(info_s.machine_loc,pchar(Edit1.Text));
   StrCopy(info_s.machine_desc,pchar(Edit2.Text));
   StrCopy(info_s.server_ip,pchar(Edit3.Text));

   if not is_short then
    begin
     StrCopy(info_f.allowed_services_ips,pchar(Edit4.Text));
     info_f.def_lang:=ComboBox2.ItemIndex;
     info_f.show_langs_at_startup:=CheckBox1.Checked;
    end;
  end;

 SendMessage(Handle, WM_SETICON, ICON_SMALL, 0);
 SendMessage(Handle, WM_SETICON, ICON_BIG, 0);

 inherited;
end;

procedure TRSStartupForm.UpdateButtons;
var page,numpages:integer;
    s_cancel,s_next,s_prev,s_finish:string;
begin
 if IsCyr then
  begin
   s_next:='����� >>';
   s_prev:='<< �����';
   s_finish:='���������';
   s_cancel:='������';
  end
 else
  begin
   s_next:='Next >>';
   s_prev:='<< Back';
   s_finish:='Finish';
   s_cancel:='Cancel';
  end;

 page:=PageControl.ActivePageIndex;
 numpages:=PageControl.PageCount;

 if page=0 then
  begin
   ButtonPrev.Visible:=false;
   ButtonNext.Visible:=true;
   ButtonNext.Caption:=s_next;
   ButtonCancel.Visible:=true;
   ButtonCancel.Caption:=s_cancel;
  end
 else
 if page=numpages-1 then
  begin
   ButtonPrev.Visible:=false;
   ButtonCancel.Visible:=false;
   ButtonNext.Visible:=true;
   ButtonNext.Caption:=s_finish;
  end
 else
  begin
   ButtonCancel.Visible:=true;
   ButtonCancel.Caption:=s_cancel;
   ButtonPrev.Visible:=true;
   ButtonPrev.Caption:=s_prev;
   ButtonNext.Visible:=true;
   ButtonNext.Caption:=s_next;
  end;

 try
  ButtonNext.SetFocus;
 except end;
end;

procedure TRSStartupForm.SwitchToPage(idx:integer);
begin
 if PageControl.ActivePageIndex<>idx then
  PageControl.ActivePageIndex:=idx;
end;

procedure TRSStartupForm.FormShow(Sender: TObject);
begin
 UpdateButtons;
 SetForegroundWindow(Handle);
end;

procedure TRSStartupForm.ButtonNextClick(Sender: TObject);
var page,numpages:integer;
begin
 page:=PageControl.ActivePageIndex;
 numpages:=PageControl.PageCount;

 if page=numpages-1 then
  begin
   can_close:=true;
   ModalResult:=mrOk;
  end
 else
  begin
   if page=2 then
    begin
     if trim(Edit3.Text)='' then
      begin
       if MessageBoxW(Handle,pwidechar(widestring('����� ������� �� ������. ����������?')),pwidechar(widestring('������')),MB_YESNO or MB_DEFBUTTON2 or MB_ICONQUESTION)<>IDYES then
        begin
         Edit3.SetFocus;
         exit;
        end;
      end;
    end;

   if not is_short then
    inc(page)
   else
    begin
     if page=2 then
      page:=5
     else
      inc(page);
    end;

   PageControl.ActivePageIndex:=page;
   UpdateButtons;
   if Assigned(PageControl.ActivePage.OnShow) then
     PageControl.ActivePage.OnShow(Sender);
  end;
end;

procedure TRSStartupForm.ButtonPrevClick(Sender: TObject);
var page:integer;
begin
 page:=PageControl.ActivePageIndex;
 if page>0 then
  begin

   if not is_short then
    dec(page)
   else
    begin
     if page=5 then
      page:=2
     else
      dec(page);
    end;
   
   PageControl.ActivePageIndex:=page;
   UpdateButtons;
   if Assigned(PageControl.ActivePage.OnShow) then
     PageControl.ActivePage.OnShow(Sender);
  end;
end;

procedure TRSStartupForm.TabSheet4Show(Sender: TObject);
var s_type,s_loc,s_desc,s_ips,s_services_ips,s_lang,s_show_lang,s_yes,s_no:string;
begin
 if IsCyr then
  begin
   s_type:='����������:';
   s_loc:='��������������:';
   s_desc:='��������:';
   s_ips:='IP-����� �������:';
   s_services_ips:='����������� IP ��� ��������:';
   s_lang:='���� �� ���������:';
   s_show_lang:='���������� ����� ������:';
   s_yes:='��';
   s_no:='���';
  end
 else
  begin
   s_type:='Machine type:';
   s_loc:='Location:';
   s_desc:='Description:';
   s_ips:='Server IP:';
   s_services_ips:='Allowed services IP:';
   s_lang:='Default language:';
   s_show_lang:='Show language selection:';
   s_yes:='Yes';
   s_no:='No';
  end;

 Memo1.Clear;
 Memo1.Lines.Add(s_type);
 Memo1.Lines.Add(' '+ComboBox1.Items[ComboBox1.ItemIndex]);
 Memo1.Lines.Add('');
 Memo1.Lines.Add(s_loc);
 Memo1.Lines.Add(' '+Edit1.Text);
 Memo1.Lines.Add('');
 Memo1.Lines.Add(s_desc);
 Memo1.Lines.Add(' '+Edit2.Text);
 Memo1.Lines.Add('');
 Memo1.Lines.Add(s_ips);
 Memo1.Lines.Add(' '+Edit3.Text);
 Memo1.Lines.Add('');

 if not is_short then
  begin
   Memo1.Lines.Add(s_services_ips);
   Memo1.Lines.Add(' '+Edit4.Text);
   Memo1.Lines.Add('');
   Memo1.Lines.Add(s_lang);
   Memo1.Lines.Add(' '+ComboBox2.Items[ComboBox2.ItemIndex]);
   Memo1.Lines.Add('');
   Memo1.Lines.Add(s_show_lang);
   if CheckBox1.Checked then
    Memo1.Lines.Add(' '+s_yes)
   else
    Memo1.Lines.Add(' '+s_no);
   Memo1.Lines.Add('');
  end;
end;

procedure TRSStartupForm.FormCloseQuery(Sender: TObject;
  var CanClose: Boolean);
begin
 CanClose:=can_close or (ButtonCancel.Visible and ButtonCancel.Enabled);
 if CanClose and ButtonCancel.Visible then
  begin
   if MessageBoxW(Handle,pwidechar(widestring('�������� ������ �������?')),pwidechar(widestring('������')),MB_YESNO or MB_DEFBUTTON2 or MB_ICONQUESTION)<>IDYES then
    CanClose:=false;
  end;
end;

procedure TRSStartupForm.TabSheet3Show(Sender: TObject);
begin
 Edit3.SetFocus;
end;

procedure TRSStartupForm.ComboBox2Change(Sender: TObject);
begin
 if not IsCyr then
  begin
   if visible then
    begin
     if ComboBox2.ItemIndex<>1 then
      begin
       MessageBoxW(Handle,'� ������������ ���������� ������� ���� ��� ��-������ �������� �� �������� ������ ���������'#13#10#13#10'������������� ���� ��� ���������� ����������, ���� ����� ������� ���������� ����','����������',MB_OK or MB_ICONWARNING);
      end;
    end;
  end;
end;

function ShowStartupMasterDialog(p:pinteger):longbool; cdecl;
var f:TRSStartupForm;
begin
 f:=TRSStartupForm.CreateForm(p);
 Result:=f.ShowModal=idok;
 f.Free;
end;


procedure TRSStartupForm.TabSheet7Show(Sender: TObject);
begin
// Edit4.SetFocus;
end;

procedure TRSStartupForm.TabSheet2Show(Sender: TObject);
begin
 ComboBox1.SetFocus;
end;

end.
