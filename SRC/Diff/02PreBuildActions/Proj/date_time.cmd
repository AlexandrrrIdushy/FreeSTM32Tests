REM get data time to var VTIME
IF "%time:~0,-10%" == " "  GOTO do_10
SET VTIME=_%date:~6%%date:~3,-5%%date:~0,-8%_%time:~0,-9%%time:~3,-6%
GOTO posle_10
:do_10
SET VTIME=_%date:~6%%date:~3,-5%%date:~0,-8%_0%time:~1,-9%%time:~3,-6%
:posle_10
SET TMSCMSC=%time:~3,-3%
