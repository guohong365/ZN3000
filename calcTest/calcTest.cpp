// calcTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../libzn/standard.h"
#include <clocale>
#include <iostream>
void ReportToConsole(const double *B, const double *Bl, const double *Bh, const double *C, const TCHAR D[][4]);

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "chs");
    double A[11];
    double B[90], Bl[90], Bh[90];
    double C[90];
    TCHAR D[90][4];
    memset(D, 0, sizeof(TCHAR)*90*4);
    for(int i = 0; i< 90; ++ i)
    {
        Bl[i] = Bh[i] = 0;
        C[i] = -1;
    }
    A[4] = 0;
    A[5] = 18; //���� 
    A[6] = 161; //��� 
    A[7] = 57; //���� 
    A[8] = 90; //����ѹ 
    A[9] = 60; //����ѹ 
    A[10] = 88; //����
    calculateStandard(A, B, Bl, Bh);
    ReportToConsole(B, Bl, Bh, C, D);
	return 0;
}

