@echo off

cd admin
call make.bat
cd..

cd client
call make.bat
cd..

cd operator
call make.bat
cd..

cd server
call make.bat
cd..

rem common must be last!
cd common
call make.bat
cd..


echo _
echo _
echo _
echo        ��������  ��   ��   �� �� ��
echo        ��������  ��  ���   �� �� ��
echo        ��    ��  �� ���    �� �� ��
echo        ��    ��  �����     �� �� ��
echo        ��    ��  �����     �� �� ��
echo        ��    ��  �� ���    �� �� ��
echo        ��������  ��  ���           
echo        ��������  ��   ��   �� �� ��
echo _
echo _
echo _
pause
