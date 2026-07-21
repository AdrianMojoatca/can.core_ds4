@echo off
cd %~dp0

SET CURRENT=%~dp0..\

SET TARGETS_FILE=%CURRENT%\targets.txt
SET GET_SELECTED=%CURRENT%\scripts\GetSelectedForBatchBuild.exe
SET PROJECT_FILE=%CURRENT%\project.uvmpw

for /f "tokens=2* " %%a in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Keil\Products\MDK" /v Path') do set "AppPath=%%~b"

SET COMPILER=%AppPath:ARM=UV4\Uv4.exe%
echo compiler: %COMPILER% 

set SVN_SET="C:\Program Files\TortoiseSVN\bin\svn.exe"
SET OUT_PATH=%CURRENT%out
SET OUT_WEB_PATH=%OUT_PATH%\web\



SET LOG_FILE=%CURRENT%log_build.txt
SET TEMP_LOG_FILE=%CURRENT%log_build_temp.txt

SET CMALL_INC_CONFIG_FW="%CURRENT%cmall\inc.config_fw"

SET DOC_FOLDER="%CURRENT%..\doc\"

SET TECH_SHEET_TEMPLATE=%OUT_WEB_PATH%tech_sheet_template.xlsx

if "%1"=="" (
%SVN_SET% update %CURRENT% --depth infinity --force --non-interactive
)

rem for /D %%i in (%OUT_PATH%\*) do %SVN_SET% delete %%i\*.enc --force
for /R %OUT_PATH% %%i in (*.enc) do del %%i

rem if exist %TECH_SHEET_TEMPLATE% (%SVN_SET% delete %TECH_SHEET_TEMPLATE% --force)
if exist %TECH_SHEET_TEMPLATE% (del %TECH_SHEET_TEMPLATE%)

rem VERIFY > nul rem to remove errorlevels

del %CURRENT%bin/s/q

del %LOG_FILE%
if exist %GET_SELECTED% (
	%GET_SELECTED% %PROJECT_FILE%
    if ERRORLEVEL 1 GOTO ErrorGetSelected
        FOR /F "tokens=*" %%i IN (%TARGETS_FILE%) DO (
            echo Building %CURRENT%%%i ...
            rem %COMPILER% -j0 -q -o  %TEMP_LOG_FILE% -r %CURRENT%%%i
            %COMPILER% -j0 -q -o  %TEMP_LOG_FILE% -b %CURRENT%%%i
            type %TEMP_LOG_FILE% >> %LOG_FILE%
        )
        del %TARGETS_FILE%
        del %TEMP_LOG_FILE%
) else (
    Echo Build All...
	%COMPILER% -j0 -q -o  %LOG_FILE% -r %PROJECT_FILE%)
GOTO LabelOk

:ErrorGetSelected
echo "Error getting selected for batch build. Will not build project."

::0 No Errors or Warnings 
::1 Warnings Only 
::2 Errors 
::3 Fatal Errors 
::11 Cannot open project file for writing 
::12 Device with given name in not found in database 
::13 Error writing project file 
::15 Error reading import XML file 

IF ERRORLEVEL   0 goto :LabelOk
IF ERRORLEVEL   1 goto :LabelOk
IF ERRORLEVEL   2 goto :LabelError

goto :LabelError

:LabelOk 
Echo Deleted all bin file...
del %CURRENT%bin/s/q

%SVN_SET% propdel svn:ignore %OUT_PATH% --recursive

xcopy %DOC_FOLDER%*tech_sheet* %OUT_WEB_PATH%tech_sheet.xlsx* /Y

rem echo "Svn Add:="%OUT_PATH%
%SVN_SET% add %OUT_PATH% --force 
IF EXIST %CMALL_INC_CONFIG_FW% (
echo "Svn Add:="%CMALL_INC_CONFIG_FW%
%SVN_SET% add %CMALL_INC_CONFIG_FW% --force 
)
REM @ECHO "Complete !!"  
goto :EOF


:LabelError
@ECHO "!! Error Found !!"

rem start notepad.exe %LOG_FILE%

goto :EOF





