#pragma once
#include <gdiplus.h>


struct Appearance
{	
	INT LineStyle;///线型
	FLOAT LineWidth;///线宽
	INT FillStyle;///填充样式
	FLOAT FontSize;///字体大小
	INT FontStyle; ///字体风格   位或  Bold=1 Italic=2 Underline=4 Strikeout=4
	INT TextAlignment;///文字对齐方式
	INT LineAlignment;///文字行对齐方式
	FLOAT Transparent;///透明度
	CString FontName; ///字体名称
	Gdiplus::Color	LineColor;///线框颜色
	Gdiplus::Color  FillColor;///填充颜色
	Gdiplus::Color FontColor; ///字体颜色
	Gdiplus::Color ActiveColor;///激活颜色
	Gdiplus::Color TraceColor;
	bool ShowBorder;  ///显示边框
	bool ShowFill;   ///填充图形
	bool ShowTrack;  ///显示操作柄
	bool ShowActive; ///显示为激活
	bool VerticalText; ///文字竖排
	Appearance();
	Appearance(const Appearance& another);
};