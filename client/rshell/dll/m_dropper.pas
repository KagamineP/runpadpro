
{*******************************************************}
{                                                       }
{       TEAP LEAP Soft Group 2004                       }
{       Dropper - Component for Drag & Drop files       }
{                                                       }
{       Copyright (c) 2004, TEAP LEAP Soft Group 2004   }
{                                                       }
{       ��������� ��� ����������� ������ �����          }
{                   Drag & Drop ���������� OLE          }
{                                                       }
{             �����������: ������� ����� ������������   }
{                           E-Mail: teap_leap@mail.ru   }
{                                                       }
{*******************************************************}

{********************** �������� ***********************}
{                                                       }
{  ��������� ������������ ��� ��������� � ���� �������  }
{ ���������� Drag & Drop, �� �� � ���� ��������� ������ }
{ �� ����������, � ��� ������ - �� ������ ������������� }
{ ����� � ��������� Windows � ������ ���������,         }
{ ������������������ � ������� DragAcceptFiles.         }
{  ��� ������������� ������� Drag & Drop (�������� ���  }
{ OnMouseDown), �������� ����� TDropper.StartDrop.      }
{ � � ������� TDropper.OnDropUp ������� ����� ���       }
{ �����������. ��������� ����� ����� ��������           }
{ Drag & Drop ������� (� ������ � ����������� - �����   }
{ ����� ������������).                                  }
{                                                       }
{  �� ����������� Drag & Drop �� ����� �������          }
{  ���������� �� ��� Delphi - ��������� ������, �� ���  }
{  �� ������� � �� ������ �� ������ �������� ���������� }
{                                                       }
{*******************************************************}

unit M_Dropper;

interface

uses
  Windows, Messages, SysUtils, Classes, ActiveX, ShlObj, Forms;

type
  { TFormatList - ������ ������� TFormatEtc }
  PFormatList = ^TFormatList;
  TFormatList = array[0..1] of TFormatEtc;

  TDropUp = procedure(var Files: TStringList) of object;
  TDropCheck  = procedure(var Allow: Boolean) of object;

  TEnumFormatEtc = class(TInterfacedObject, IEnumFormatEtc)
  private
    FFormatList  : PFormatList;
    FFormatCount : Integer;
    FIndex       : Integer;
  public
    constructor Create(FormatList: PFormatList; FormatCount, Index: Integer);
    { IEnumFormatEtc }
    function Next(Celt: LongInt; out Elt; PCeltFetched: PLongInt): HResult; stdcall;
    function Skip(Celt: LongInt): HResult; stdcall;
    function Reset: HResult; stdcall;
    function Clone(out Enum: IEnumFormatEtc): HResult; stdcall;
  end;

  { TDragDropInfo ���������� � ��������������� ������ }
  TDragDropInfo = class(TObject)
  private
    FInClientArea : Boolean;
    FDropPoint    : TPoint;
    FFileList     : TStringList;
  public
    constructor Create(ADropPoint : TPoint; AInClient : Boolean);
    function    CreateHDrop : HGlobal;
    destructor  Destroy; override;
    procedure   Add(const s : String);

    property InClientArea : Boolean     read FInClientArea;
    property DropPoint    : TPoint      read FDropPoint;
    property Files        : TStringList read FFileList;
  end;

  TFileDropEvent = procedure(DDI : TDragDropInfo) of object;

  { THDropDataObject - ������ ������ �
    ����������� � ��������������� ������ }
  THDropDataObject = class(TInterfacedObject, IDataObject)
  private
    FDropInfo : TDragDropInfo;
  public
    constructor Create(ADropPoint: TPoint; AInClient: Boolean);
    destructor  Destroy; override;
    procedure   Add(const s : String);
    { �� IDataObject }
    function GetData(const formatetcIn: TFormatEtc;
        out medium: TStgMedium): HResult; stdcall;
    function GetDataHere(const formatetc: TFormatEtc;
        out medium: TStgMedium): HResult; stdcall;
    function QueryGetData(const formatetc: TFormatEtc): HResult; stdcall;
    function GetCanonicalFormatEtc(const formatetc: TFormatEtc;
        out formatetcOut: TFormatEtc): HResult; stdcall;
    function SetData(const formatetc: TFormatEtc; var medium: TStgMedium;
        fRelease: BOOL): HResult; stdcall;
    function EnumFormatEtc(dwDirection: Longint;
        out enumFormatEtc: IEnumFormatEtc): HResult; stdcall;
    function DAdvise(const formatetc: TFormatEtc; advf: Longint;
        const advSink: IAdviseSink; out dwConnection: Longint): HResult; stdcall;
    function DUnadvise(dwConnection: Longint): HResult; stdcall;
    function EnumDAdvise(out enumAdvise: IEnumStatData): HResult; stdcall;
  end;

  { TFileDropSource - ��������
    ��� �������������� ������ }
  TFileDropSource = class(TInterfacedObject, IDropSource)
    FOnDragQuery : TDropUp;
    FOnDropCheck : TDropCheck;
    constructor Create;
    function QueryContinueDrag(fEscapePressed: BOOL;
        grfKeyState: Longint): HResult; stdcall;
    function GiveFeedback(dwEffect: Longint): HResult; stdcall;
    property OnDragQuery : TDropUp read FOnDragQuery write FOnDragQuery;
    property OnDropCheck : TDropCheck read FOnDropCheck write FOnDropCheck;
  end;

  TDropper = class(TComponent)
  private
    { Private declarations }
    FEnabled     : Boolean;
    FIsDropping  : Boolean;
    FOnDropUp    : TDropUp;
    FOnDropCheck : TDropCheck;
  protected
    { Protected declarations }
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    procedure   StartDrag;
    property    IsDropping : Boolean read FIsDropping;
  published
    { Published declarations }
    property Enabled     : Boolean    read FEnabled     write FEnabled;
    property OnDropUp    : TDropUp    read FOnDropUp    write FOnDropUp;
    property OnDropCheck : TDropCheck read FOnDropCheck write FOnDropCheck;
  end;

var
  DropData : THDropDataObject;
  Enabld   : Boolean;

procedure Register;

implementation

// TEnumFormatEtc //////////////////////////////////////////////////////////////

constructor TEnumFormatEtc.Create(FormatList: PFormatList;
    FormatCount, Index : Integer);
begin
  inherited Create;
  FFormatList := FormatList;
  FFormatCount := FormatCount;
  FIndex := Index;
end;

{
  Next ��������� �������� ����������
  �������� TFormatEtc
  � ������������ ������ elt.
  ����������� celt ���������, ������� �
  ������� ������� � ������.
}
function TEnumFormatEtc.Next(celt: Longint; out elt; pceltFetched: PLongint): HResult;
var
  i : Integer;
  eltout : TFormatList absolute elt;
begin
  i := 0;

  while (i < celt) and (FIndex < FFormatCount) do
  begin
    eltout[i] := FFormatList[FIndex];
    Inc (FIndex);
    Inc (i);
  end;

  if (pceltFetched <> nil) then
    pceltFetched^ := i;

  if (I = celt) then
    Result := S_OK
  else
    Result := S_FALSE;
end;

{
  Skip ���������� celt ��������� ������, 
  ������������ ������� �������
  �� (CurrentPointer + celt) ��� �� ����� 
  ������ � ������ ������������.
}
function TEnumFormatEtc.Skip(celt: Longint): HResult;
begin
  if (celt <= FFormatCount - FIndex) then
  begin
    FIndex := FIndex + celt;
    Result := S_OK;
  end else
  begin
    FIndex := FFormatCount;
    Result := S_FALSE;
  end;
end;

{ Reset ������������� ��������� �������
������� �� ������ ������ }
function TEnumFormatEtc.Reset: HResult;
begin
  FIndex := 0;
  Result := S_OK;
end;

{ Clone �������� ������ �������� }
function TEnumFormatEtc.Clone(out enum: IEnumFormatEtc): HResult;
begin
  enum := TEnumFormatEtc.Create 
  (FFormatList, FFormatCount, FIndex);
  Result := S_OK;
end;

// TFileDropSource /////////////////////////////////////////////////////////////

constructor TFileDropSource.Create;
begin
  inherited Create;
  _AddRef;
end;


function TFileDropSource.QueryContinueDrag(fEscapePressed: BOOL;
    grfKeyState: Longint): HResult;
var
  Files     : TStringList;
  I         : Integer;
  AllowDrop : Boolean;
begin
  if not Enabld then
    begin
      Result := DRAGDROP_S_CANCEL;
      Exit;
    end;

  if (fEscapePressed) then
    Result := DRAGDROP_S_CANCEL
  else
     if (grfKeyState and MK_LBUTTON) = 0 then
       begin
         if Assigned(FOnDropCheck) then
           FOnDropCheck(AllowDrop)
         else
           AllowDrop := true;

         if AllowDrop then
           begin
             Files := TStringList.Create;
             if Assigned(FOnDragQuery) then      // ���������� ������ ������
               FOnDragQuery(Files);
             for i := 0 to Files.Count - 1 do
               DropData.Add(Files.Strings[i]);
             Files.Free;

             Result := DRAGDROP_S_DROP;
           end
         else
           Result := DRAGDROP_S_CANCEL;

       end
     else Result := S_OK;
end;

function TFileDropSource.GiveFeedback(dwEffect: Longint): HResult;
var
  i: DWORD;
  AllowDrop: Boolean;
begin
  if Assigned(FOnDropCheck) then
    FOnDropCheck(AllowDrop)
  else
    AllowDrop := true;

  if AllowDrop then
    begin
      i:=dwEffect;
      case i of
        DROPEFFECT_NONE,
        DROPEFFECT_COPY,
        DROPEFFECT_LINK,
        DROPEFFECT_SCROLL : Result := DRAGDROP_S_USEDEFAULTCURSORS;
        else
          Result := S_OK;
      end;
    end
  else
    begin
      SetCursor(LoadCursor(0, IDC_NO));
      Result := S_OK;
    end;
end;

{ THDropDataObject }
constructor THDropDataObject.Create(ADropPoint : TPoint; AInClient : Boolean);
begin
  inherited Create;
  _AddRef;
  FDropInfo := TDragDropInfo.Create(ADropPoint, AInClient);
end;

destructor THDropDataObject.Destroy;
begin
  if (FDropInfo <> nil) then FDropInfo.Free;
  inherited Destroy;
end;

procedure THDropDataObject.Add(const s : String);
begin
  FDropInfo.Add(s);
end;

function THDropDataObject.GetData(const formatetcIn: TFormatEtc;
  out medium: TStgMedium): HResult;
begin
  Result := DV_E_FORMATETC;
  { ���������� �������� ��� ���� medium  �� ������ ������}
  medium.tymed := 0;
  medium.hGlobal := 0;
  medium.unkForRelease := nil;

  { ���� ������ ��������������, �������
    � ���������� ������ }
  if (QueryGetData(formatetcIn) = S_OK) then
  begin
    if (FDropInfo <> nil) then
    begin
      medium.tymed := TYMED_HGLOBAL;
      { �� ������������ �������� ���������� �������! }
      medium.hGlobal := FDropInfo.CreateHDrop;
      Result := S_OK;
    end;
  end;
end;

function THDropDataObject.GetDataHere( const formatetc: TFormatEtc;
    out medium: TStgMedium): HResult;
begin
  Result := DV_E_FORMATETC;  { � ���������, �� �������������� }
end;

function THDropDataObject.QueryGetData(const formatetc: TFormatEtc): HResult;
begin
  Result := DV_E_FORMATETC;
  with formatetc do
    if dwAspect = DVASPECT_CONTENT then
      if (cfFormat = CF_HDROP) and (tymed = TYMED_HGLOBAL) then
        Result := S_OK;
end;

function THDropDataObject.GetCanonicalFormatEtc(const formatetc: TFormatEtc;
   out formatetcOut: TFormatEtc): HResult;
begin
  formatetcOut.ptd := nil;
  Result := E_NOTIMPL;
end;

function THDropDataObject.SetData(const formatetc: TFormatEtc;
    var medium: TStgMedium; fRelease: BOOL): HResult;
begin
  Result := E_NOTIMPL;
end;

// TDragDropInfo ///////////////////////////////////////////////////////////////

constructor TDragDropInfo.Create(ADropPoint : TPoint; AInClient : Boolean);
begin
  inherited Create;
  FFileList     := TStringList.Create;
  FDropPoint    := ADropPoint;
  FInClientArea := AInClient;
end;

destructor TDragDropInfo.Destroy;
begin
  FFileList.Free;
  inherited Destroy;
end;

function TDragDropInfo.CreateHDrop : HGlobal;
var
  RequiredSize : Integer;
  i : Integer;
  hGlobalDropInfo : HGlobal;
  DropFiles : PDropFiles;
  c : PChar;
begin
  {
    �������� ��������� TDropFiles � ������, 
    ���������� �����
    GlobalAlloc. ������� ������ ������� ���������� 
    � ����������,
    ��������� ���, ��������, ����� ������������ 
    ������� ��������.
  }

  { ���������� ����������� ������ ��������� }
  RequiredSize := SizeOf (TDropFiles);
  for i := 0 to Self.Files.Count-1 do
  begin
    { ����� ������ ������, ���� 1 ���� ��� ����������� }
    RequiredSize := RequiredSize + Length(Self.Files[i]) + 1;
  end;
  { 1 ���� ��� ������������ ����������� }
  Inc(RequiredSize);

  hGlobalDropInfo := GlobalAlloc((GMEM_SHARE or GMEM_MOVEABLE or GMEM_ZEROINIT),
                       RequiredSize);
  if (hGlobalDropInfo <> 0) then
  begin
    { ����������� ������� ������, ����� � ���
      ����� ���� ���������� }
    DropFiles := GlobalLock(hGlobalDropInfo);

    { �������� ���� ��������� DropFiles }
    { pFiles -- �������� �� ������
      ��������� �� ������� ����� �������
      � ������� ������. }
    DropFiles.pFiles := SizeOf(TDropFiles);
    DropFiles.pt     := Self.FDropPoint;
    DropFiles.fNC    := Self.InClientArea;
    DropFiles.fWide  := False;

    { �������� ������ ��� ����� � �����.
      ����� ���������� �� ��������
      DropFiles + DropFiles.pFiles,
      �� ���� ����� ���������� ���� ���������. }
    c := PChar(DropFiles);
    c := c + DropFiles.pFiles;
    for i := 0 to Self.Files.Count-1 do
    begin
      StrCopy(c, PChar(Self.Files[i]+#0));
      c := c + Length(Self.Files[i]) + 1;
    end;

    { ������� ���������� }
    GlobalUnlock (hGlobalDropInfo);
  end;

  Result := hGlobalDropInfo;
end;

procedure TDragDropInfo.Add(const s : String);
begin
  Files.Add(S);
end;

// EnumFormatEtc ///////////////////////////////////////////////////////////////

{ EnumFormatEtc ���������� ������
�������������� �������� }
function THDropDataObject.EnumFormatEtc(dwDirection: Longint;
    out enumFormatEtc: IEnumFormatEtc): HResult;
const
  DataFormats: array[0..0] of TFormatEtc =
  ((
      cfFormat : CF_HDROP;
      ptd      : nil;
      dwAspect : DVASPECT_CONTENT;
      lindex   : -1;
      tymed    : TYMED_HGLOBAL;
   ));
  DataFormatCount = 1;
begin
  { �������������� ������ Get. ������
  ���������� ������ ������ }
  if dwDirection = DATADIR_GET then
  begin
    enumFormatEtc := TEnumFormatEtc.Create(@DataFormats, DataFormatCount, 0);
    Result := S_OK;
  end else
  begin
    enumFormatEtc := nil;
    Result := E_NOTIMPL;
  end;
end;

{ ������� Advise �� �������������� }
function THDropDataObject.DAdvise(const formatetc: TFormatEtc; advf: Longint;
    const advSink: IAdviseSink; out dwConnection: Longint): HResult;
begin
  Result := OLE_E_ADVISENOTSUPPORTED;
end;

function THDropDataObject.DUnadvise(dwConnection: Longint): HResult;
begin
  Result := OLE_E_ADVISENOTSUPPORTED;
end;

function THDropDataObject.EnumDAdvise(out enumAdvise: IEnumStatData): HResult;
begin
  Result := OLE_E_ADVISENOTSUPPORTED;
end;

// TDropper ////////////////////////////////////////////////////////////////////

constructor TDropper.Create(AOwner: TComponent);
begin
 inherited Create(AOwner);
end;

procedure TDropper.StartDrag;
var
  DropSource : TFileDropSource;
  Rslt       : HRESULT;
  dwEffect   : Integer;
  DropPoint  : TPoint;
begin
 try
   { ������� ������-��������... }
   DropSource := TFileDropSource.Create;

   { ...� ������ ������ }
   DropPoint.x := 100;
   DropPoint.y := 100;
   DropData := THDropDataObject.Create(DropPoint, True);

   DropSource.OnDragQuery := OnDropUp;
   DropSource.OnDropCheck := OnDropCheck;

   Enabld  := FEnabled;
   FIsDropping := True;

   Rslt := DoDragDrop(DropData, DropSource, DROPEFFECT_COPY, dwEffect);

  // DropData.Free;

   if ((Rslt <> DRAGDROP_S_DROP) and (Rslt <> DRAGDROP_S_CANCEL)) then
     case rslt of
       E_OUTOFMEMORY : raise Exception.Create('������������ ������ ��� Drag & Drop');
         else raise Exception.Create('��� Drag & Drop ��������� ������');
     end;

   { ����������� �������������� ������� ����� ���������� ������ }
   DropSource.Free;
   DropData.Free;

   FIsDropping := False;
 except
 end;
end;

procedure Register;
begin
  RegisterComponents('Samples', [TDropper]);
end;

//initialization
//  OleInitialize(nil);

//finalization
//  OleUninitialize;

end.
