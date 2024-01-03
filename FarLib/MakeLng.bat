@echo off

if (%1) == () (
	for %%* in (.) do set NAME=%%~nx*
) else (
	set NAME=%1
)

if (%2) == () (
	set HEADER=%NAME%.h
) else (
	set HEADER=%2
)

python "%~dp0\MakeMsg.py" "%NAME%.msg" "%HEADER%" %NAME%Eng.lng %NAME%Rus.lng %3&
