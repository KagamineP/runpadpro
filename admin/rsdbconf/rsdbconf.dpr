program rsdbconf;

uses
  Forms,
  main in 'main.pas' {DBConfForm};

{$R *.res}
{$R resource.res}

begin
  Application.Initialize;
  Application.Title := '������������ ���� ������ Runpad Pro';
  Application.CreateForm(TDBConfForm, DBConfForm);
  Application.Run;
end.
