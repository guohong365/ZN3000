#include "stdafx.h"
#include <uc/drawing/DrawObjectGlobal.h>
const uc::drawing::FONT_SIZE FontSizeArray[]={
	{_T("����"),	42},
	{_T("С��"),	36},
	{_T("һ��"),	26},
	{_T("Сһ"),	24},
	{_T("����"),	22},
	{_T("С��"),	18},
	{_T("����"),	16},
	{_T("С��"),	15},
	{_T("�ĺ�"),	14},
	{_T("С��"),	12},
	{_T("���"),	10.5},
	{_T("С��"),	9},
	{_T("����"),	7.5},
	{_T("С��"),	6.5},
	{_T("�ߺ�"),	5.5},
	{_T("�˺�"),	5},
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