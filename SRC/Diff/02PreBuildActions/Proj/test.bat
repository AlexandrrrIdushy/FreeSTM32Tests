echo %CntRunBuil%
echo %CntRunBuil%
if not defined CntRunBuil (
	echo CntRunBuil is NOT defined. do it
	SET CntRunBuil=1
	rem setenv -u CntRunBuil 0
	rem setx CntRunBuil=0
	rem SET /A CntRunBuil=0
)
rem SET /A CntRunBuil=CntRunBuil+1
echo %CntRunBuil%