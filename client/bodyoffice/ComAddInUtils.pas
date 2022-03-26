unit ComAddInUtils;

interface

uses Windows, ActiveX, Office_TLB, SinkObject, ComObj;

type
  // ���������� �������, ���������� �� ������� �� ������
  TOnCommandButtonClick = procedure (Button: CommandBarButton;
     var CancelDefault: WordBool) of object;

  TCommandButtonEventSink = class(TBaseSink)
  private
    FOnClick: TOnCommandButtonClick;
  protected
    procedure DoClick(Button: CommandBarButton;
      var CancelDefault: WordBool); virtual;
    function DoInvoke (DispID: Integer; const IID: TGUID; LocaleID: Integer;
      Flags: Word; var dps : TDispParams; pDispIds : PDispIdList;
      VarResult, ExcepInfo, ArgErr: Pointer): HResult; override;
  public
    constructor Create; virtual;
    property OnClick: TOnCommandButtonClick read FOnClick write FOnClick;
  end;


implementation

{ TCommandButtonEventSink }

constructor TCommandButtonEventSink.Create;
begin
  inherited;
  // ������������� ���������� ������������� ��������� �������
  FSinkIID := _CommandBarButtonEvents;
end;

procedure TCommandButtonEventSink.DoClick(Button: CommandBarButton;
  var CancelDefault: WordBool);
begin
  if Assigned(FOnClick) then
    FOnClick(Button, CancelDefault);
end;

function TCommandButtonEventSink.DoInvoke(DispID: Integer;
  const IID: TGUID; LocaleID: Integer; Flags: Word; var dps: TDispParams;
  pDispIds: PDispIdList; VarResult, ExcepInfo, ArgErr: Pointer): HResult;
begin
//  ���� ����� ����� ���������� ��� ������������� �������
//  � ���� ����� ���� �������� �������������� DispId, ��������������� ���������������
//  ������� ����������, � �������� �� ����������
//
//  _CommandBarButtonEvents = dispinterface
//    ['{000C0351-0000-0000-C000-000000000046}']
//    procedure Click(const Ctrl: CommandBarButton; var CancelDefault: WordBool); dispid 1;
//  end;
//
//  ����� ������� ���� ����������� ����� � DispId = 1
//  � ����� ����������� ���� CommandBarButton � WordBool
  Result := S_OK;
  case DispID of
    1 : DoClick(IUnknown (dps.rgvarg^ [pDispIds^ [0]].unkval) as CommandBarButton, 
          dps.rgvarg^ [pDispIds^ [1]].pbool^);
  else
    Result := DISP_E_MEMBERNOTFOUND;
  end;
end;

end.
