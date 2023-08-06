@echo off
del /Q log.log
del /Q settings.cfg
del /S /Q Build\*
rmdir /S /Q Build\Output
rmdir /S /Q Build\Win32GUISample.tlog
pause