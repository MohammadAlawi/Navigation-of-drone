::============================================================
:: This script runs Wifi Signal strength test.
:: Signal is good when: 
:: Signal >= 80%
::============================================================
@echo off
:start
set /a var+=1
if %var% EQU 10 goto end
netsh wlan show interfaces
timeout 1
goto start

:end
echo var has reached %var%.
pause
exit

:: Ignore
:: Desktop\Check_Wifi.bat