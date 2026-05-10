@ECHO OFF
REM Check arguments
IF "%1%"=="" GOTO SHOWHELP
REM
REM --- Switch Experion System theme
pushd "%HwInstallPath%\Experion PKS\Client\System\R511\Themes"
changetheme.exe %1%
popd
REM --- Switch SP CSS theme
IF EXIST "%HwProgramData%\Experion PKS\Client\Abstract\CSS" (
 pushd "%HwProgramData%\Experion PKS\Client\Abstract\CSS"
 call sp_%1%.bat
 popd
)
REM --- Switch SP TrendPopups theme
IF EXIST "%HwProgramData%\Experion PKS\Client\Abstract\Themes\%1%" (
 pushd "%HwProgramData%\Experion PKS\Client\Abstract"
 xcopy  .\Themes\%1%  . /S /Y /I > nul
 popd
)

GOTO END

:SHOWHELP
ECHO.
ECHO ChangeTheme SP
ECHO =====================
ECHO.
ECHO To change the theme for Solution Pack CSS and Trend/MiniTrend popups enter the new theme as the first argument
ECHO.
ECHO.
ECHO Example:
ECHO ---------------------
ECHO.
ECHO               ChangeThemeSP.bat [Theme]
ECHO               Where [Theme] is the name of the theme, Light/Dark/Classic
GOTO END

:END
REM --- EXIT
exit /b