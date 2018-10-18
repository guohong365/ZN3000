#include "stdafx.h"

#include "constants.h"
#include <clocale>
#include <iostream>
#include <iomanip>
#define VALUE_COUNT 81
using namespace std;
struct ReportColumn
{
    const TCHAR * title;
    int width;
};

static void calcColumnWidth(ReportColumn * columns)
{
    int count=VALUE_COUNT;
    int length=_tcslen(GlobalNames[0]);
    for(int i=1; i< count; ++i)
    {
        int len=_tcslen(GlobalNames[i]);
        if(len > length) length = len;
    }    
    columns[0].width = length + 1;
    length = _tcslen(GlobalUnits[0]);
    for(int i=1; i< count; ++i)
    {
        int len=_tcslen(GlobalUnits[i]);
        if(len > length) length = len;
    }
    columns[1].width = length + 2;
    columns[2].width = columns[4].width = 11;
    columns[3].width = 14;
    columns[5].width = 4;

}
void ReportToConsole(const double *B, const double *Bl, const double *Bh, const double *C, const TCHAR D[][4])
{
    setlocale(LC_ALL, "chs");
    ReportColumn columns[6];
    columns[0].title = GlobalColumnTitles[0];
    columns[1].title = GlobalColumnTitles[1];
    columns[2].title = GlobalColumnTitles[2];
    columns[3].title = GlobalColumnTitles[3];
    columns[4].title = GlobalColumnTitles[4];
    columns[5].title = GlobalColumnTitles[5];
    calcColumnWidth(columns);
    wcout.setf(ios::left);
    for(int i=0; i < 6; ++i)
    {
        wcout << setw(columns[i].width) << columns[i].title;
    }
    wcout << endl;
    int count = VALUE_COUNT;
    for(int i= 1; i< count; ++i)
    {
        wcout <<  setw(columns[0].width) << GlobalNames[i];
        wcout << setw(columns[1].width) << GlobalUnits[i];
        wcout << setw(columns[2].width) << setprecision(4) << B[i];
        wcout << setw(columns[3].width) << setprecision(4) << Bl[i] << _T(" - ") << setprecision(4) << Bh[i];
        if(C[i] < 0)
        {
            wcout << setw(columns[4].width) << _T(" Nan ");
            wcout << setw(columns[5].width) << _T("");
        }
        else
        {
            wcout << setw(columns[4].width) << setprecision(4) << C[i];
            wcout << setw(columns[5].width) << D[i];
        }
        wcout << endl;
    }
    wcout << endl;
}