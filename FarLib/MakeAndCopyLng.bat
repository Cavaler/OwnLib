@echo off
call %~dp0\MakeLng.bat %*
echo Copying resulting language files...
if NOT "%FAR_DEBUG_DLLS%"=="" (
  copy *.lng "%FAR_DEBUG_DLLS%\%1\" 2>nul
)
if NOT "%FAR2_DEBUG_DLLS%"=="" (
  copy *.lng "%FAR2_DEBUG_DLLS%\%1\" 2>nul
)
if NOT "%FAR3_DEBUG_DLLS%"=="" (
  copy *.lng "%FAR3_DEBUG_DLLS%\%1\" 2>nul
)
