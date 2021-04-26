::============================================================
:: This script runs Wifi Signal strength test.
:: Signal is good when: 
:: Signal >= 80%
::============================================================
@echo off
:start
set /a var+=1
if %var% EQU 1000 goto end
echo *********************************************************
netsh wlan show interfaces
echo *********************************************************
timeout 1
echo *********************************************************
goto start

:end
echo var has reached %var%. Program ended.
pause
exit

:: Ignore
:: Desktop\Check_Wifi.bat