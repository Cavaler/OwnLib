@echo off
call %~dp0\MakeLng.bat %*

echo Copying resulting language files...
SETLOCAL EnableDelayedExpansion

for %%V in (FAR_DEBUG_DLLS FAR2_DEBUG_DLLS FAR3_DEBUG_DLLS FAR_64_DEBUG_DLLS FAR2_64_DEBUG_DLLS FAR3_64_DEBUG_DLLS) do (
  if NOT "!%%V!"=="" (
    echo Copy %%V to !%%V!\%1
    copy *.lng "!%%V!\%1\" 2>nul
  )
)
