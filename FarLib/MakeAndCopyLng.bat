@echo off
call %~dp0\MakeLng.bat %$
echo Copying resulting language files...
copy *.lng "%FAR_DEBUG_DLLS%\%1\" 2>nul
copy *.lng "%FAR2_DEBUG_DLLS%\%1\" 2>nul
copy *.lng "%FAR3_DEBUG_DLLS%\%1\" 2>nul
