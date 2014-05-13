@echo off
set INPUT_DIR=%cd%\bin\data\
set OUTPUT_DIR=%cd%\bin\data\
cd tools
cd pngtodds
dir /B /ON %INPUT_DIR%\*.png > #png.txt
for /F %%i in (#png.txt) do (
	texconv.exe -f BC3_UNORM -pmalpha %INPUT_DIR%\%%i
)
del /F/Q/S #png.txt > NUL
dir /B /ON %cd%\*.dds > #dds.txt
for /F %%i in (#dds.txt) do (
	move %%i %OUTPUT_DIR%\
)
del /F/Q/S #dds.txt > NUL
pause
