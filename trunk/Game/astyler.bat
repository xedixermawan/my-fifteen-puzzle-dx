@echo off
SET ASTYLE_DIR=%cd%\tools
SET SRC_ROOT_DIR=%cd%\src
echo ..AStyle beautfying...
%ASTYLE_DIR%\AStyle.exe --style=google --indent=spaces=4 -xn -xc -xl -xk -xe -n --align-pointer=type --recursive %SRC_ROOT_DIR%\*.h %SRC_ROOT_DIR%\*.cpp %SRC_ROOT_DIR%\*.hpp %SRC_ROOT_DIR%\*.c
echo ..done..
rem echo ..SVN Comitting..
rem svn commit %SRC_ROOT_DIR% --message "auto commit: Astyle auto beautify code "
rem echo ..done..