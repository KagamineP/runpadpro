# Исходный код Runpad Pro Shell
Данный репозиторий является форком неизмененного исходного кода Runpad Pro Shell на случай, если оригинальный репозиторий будет удален  
Ссылка на оргинальный пакет build_tools и compilers будет позже.

## Подготовка окружения для разработки
1. В настройках системы, в разделе «Язык программ, не поддерживающих Юникод» должен быть указан русский язык. 
2. В переменной PATH необходимо добавить путь к папке ```build_tools```. 
3. Нужно установить HTML Help Workshop, после чего в PATH добавить к нему путь. 
4. Установить Delphi 7.
* При установке нужно выбрать Typical Setup и снять галочку с Corba и Visibroker. 
* После установки Delphi 7 к нему необходимо прописать путь в PATH. 
5. Нужно будет установить обнолвние для Delphi 7. Он будет лежать в папке compilers.
6. В переменной PATH нужно добавить путь к компилятору Visual C++. Пример:
  ```
  C:\vc\vc\bin;C:\vc\Common7\IDE;C:\vc\vc\PlatformSDK\Bin;
  ```
7. В системные переменные необходимо добавить следующие переменные:
```
VCDIR=C:\vc\vc
VCIDEDIR=C:\vc\Common7\IDE
VCBINDIR=%VCDIR%\bin
VCBIN64DIR=%VCDIR%\bin\x86_amd64
INCLUDE=%VCDIR%\Include;%VCDIR%\PlatformSDK\Include;%VCDIR%\ATLMFC\Include
LIB32=%VCDIR%\Lib;%VCDIR%\PlatformSDK\Lib;%VCDIR%\ATLMFC\Lib
LIB64=%VCDIR%\Lib\amd64;%VCDIR%\PlatformSDK\Lib\x64;%VCDIR%\ATLMFC\Lib\amd64
LIB=%LIB32%
CL=-nologo
```
* Примечание! Где C, там нужно указать ту букву раздела, куда Вы распаковали папку vc

## Сборка
В корне лежит два bat-файла.
make.bat отвечает за компиляцию всего комплекса, а install.bat для создания установщика.
