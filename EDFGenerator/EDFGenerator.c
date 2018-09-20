// EDFGenerator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

extern int test_main();
extern int sine_main();
extern int sweep_main();
void help()
{
	printf("usage :EDFGenerator [hstw]");
}
int _tmain(int argc, _TCHAR* argv[])
{
		if(argc >=2)
	{
		switch (argv[1][0])
		{
		case _T('s'):
			sine_main();
			break;
		case _T('t'):
			test_main();
			break;
		case _T('w'):
			sweep_main();
			break;
		case _T('h'):
			help();
			break;
		default:
			sine_main();
			break;
		}
	}
	return 0;
}

