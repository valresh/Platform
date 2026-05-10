@REM
@REM	03/14/02 - gbg - updated for R500
@REM	08/07/02 - gbg - Experion PKS name change (new input arg)
@REM	10/02/02 - gbg - Default path = r100
@REM	03/18/03 - gbg - Gemini, default path = r200
@REM    04/26/04 - rjm - Mercury, default path = r210
@REM    02/22/05 - rjm - Saturn, default path = r300
@REM    04/16/07 - rjm - R310, default path changed to r310 from r300.


set COMMON_PATH=c:\Program Files\Honeywell\Experion PKS
IF NOT ""=="%1"  set COMMON_PATH=%1

"%COMMON_PATH%\client\dspbld\ETCH.EXE" /regserver
C:\WINNT\System32\TOOLBOX.EXE /regserver
REM @ECHO OFF
@ECHO *
@ECHO * About to Register Process System Detail Displays....
@ECHO *
REM C:\Honeywell\TPS50\Uninst\misc\Sleep 60
for %%f in ("%COMMON_PATH%\client\system\r310\sysshaaga*.dsp") do "%COMMON_PATH%\client\dspbld\dspbld" -e "%%f"
for %%f in ("%COMMON_PATH%\client\system\r310\sysscda*.dsp") do "%COMMON_PATH%\client\dspbld\dspbld" -e "%%f"
"%COMMON_PATH%\client\dspbld\dspbld" -e "%COMMON_PATH%\client\system\r310\SYSq476.DSP"
