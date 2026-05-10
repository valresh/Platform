@REM	08/07/02 - gbg - Experion PKS name change (new input arg

set COMMON_PATH=c:\Program Files\Honeywell\Experion PKS
IF NOT ""=="%1"  set COMMON_PATH=%1

for %%f in ("%COMMON_PATH%\client\abstract\aga_displays\sysshaaga1*.dsp") do "%COMMON_PATH%\client\dspbld\dspbld" -e "%%f"

