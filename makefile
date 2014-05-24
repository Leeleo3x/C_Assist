test.exe : test.c views\objects.c modules\mouse.c modules\system.c CLex\CLex.c CPrint\CPrint.c indent\indent.c
	wcl386 /fe=test.exe test.c views\objects.c modules\mouse.c modules\system.c CLex\CLex.c CPrint\CPrint.c indent\indent.c

