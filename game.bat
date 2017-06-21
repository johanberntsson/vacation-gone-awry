@echo off
..\bin\i615.exe +..\lib vacation > err.txt
if errorlevel 1 goto uncool
..\bin\frotz.exe vacation.z5
cls
:uncool
type err.txt | more


