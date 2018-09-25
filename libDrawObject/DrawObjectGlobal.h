#pragma once

#define DEFAULT_TEXT_COLOR        RGB(0,0,0)
#define DEFAULT_LINE_COLOR        RGB(0,0,0)
#define DEFAULT_FILL_COLOR        RGB(255,255,255)
#define DEFAULT_FILL_STYLE        0 //SOLID 
#define DEFAULT_ACTIVE_COLOR      RGB(255,0,0)
#define DEFAULT_TRACK_COLOR       RGB(0,255,0)
#define DEFAULT_LINE_STYLE        0 //SOLID
#define DEFAULT_LINE_WIDTH        1
#define DEFAULT_ARROW_TYPE		  0 //No arrow
#define DEFAULT_TRANSPARENT		  1.0f

#define FONT_SIZE_COUNT           32
#define DEFAULT_FONT_STYLE         0  //regular
#define DEFAULT_FONT_NAME          _T("ËÎÌו")
#define DEFAULT_FONT_SIZE_INDEX    9
#define DEFAULT_FONT_SIZE          12*3.53f
#define DEFAULT_FONT_COLOR         0xFF000000


struct FONT_SIZE
{
	TCHAR SizeName[20];
	float SizePoint;
};

extern const FONT_SIZE FontSizeArray[];

int AfxGetFontSizeBySizePoint(float point);

int AfxGetFontSizeBySizeName(const CString & sizeName);

