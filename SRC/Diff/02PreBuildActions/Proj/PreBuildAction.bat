rem STAGE1: svn number revision to header
rem in Build Actions Configuration mast be add string:PreBuildAction.bat $PROJ_DIR$
copy ..\RevisionsInfo_.h ..\RevisionsInfo.h
SubWCRev.exe ..\ ..\revisions_info_h.tmpl ..\RevisionsInfo.h -f
if errorlevel 1 echo trouble with svn

rem STAGE2: current time string to header
call ..\date_time.cmd
echo DATE_TIME_BUILT	equ	DT%VTIME%>>"..\RevisionsInfo.h"

rem STAGE3: time marker actual debug to header
echo //метка актуальности последнего построени€
echo TIME_MARK_BUILT	equ	%TMSCMSC%>>"..\RevisionsInfo.h"
