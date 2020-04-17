REM get data time to var VTIME
rem SET VTIME=_%date:~6%%date:~3,-5%%date:~0,-8%_0%time:~1,-9%%time:~3,-6%
SET VTIME=%time:~3,-3%
echo %VTIME%