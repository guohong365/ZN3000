#include "stdafx.h"
#include <uc/drawing/DrawObjectGlobal.h>


const uc::drawing::FONT_SIZE FontSizeArray[]={
	{_T("初号"),	42},
	{_T("小初"),	36},
	{_T("一号"),	26},
	{_T("小一"),	24},
	{_T("二号"),	22},
	{_T("小二"),	18},
	{_T("三号"),	16},
	{_T("小三"),	15},
	{_T("四号"),	14},
	{_T("小四"),	12},
	{_T("五号"),	10.5},
	{_T("小五"),	9},
	{_T("六号"),	7.5},
	{_T("小六"),	6.5},
	{_T("七号"),	5.5},
	{_T("八号"),	5},
	{_T("8"),		8},
	{_T("9"),		9},
	{_T("10"),		10},
	{_T("11"),		11},
	{_T("12"),		12},
	{_T("14"),		14},
	{_T("16"),		16},
	{_T("18"),		18},
	{_T("20"),		20},
	{_T("22"),		22},
	{_T("24"),		24},
	{_T("26"),		26},
	{_T("28"),		28},
	{_T("36"),		36},
	{_T("48"),		48},
	{_T("72"),		72}
};

int uc::drawing::AfxGetFontSizeBySizePoint(float point)
{
	for(int i=0; i<FONT_SIZE_COUNT; i++)
	{
		if(point==FontSizeArray[i].SizePoint)
		{
			return i;
		}
	}
	return -1;
}
int uc::drawing::AfxGetFontSizeBySizeName(const CString & sizeName)
{
	for(int i=0; i< FONT_SIZE_COUNT; i++)
	{
		if(sizeName==FontSizeArray[i].SizeName)
		{
			return i;
		}
	}
	return -1;
}

void AfxInitObjectsContextMenu()
{

}