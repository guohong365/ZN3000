#pragma once
#include <gdiplus.h>


struct Appearance
{	
	INT LineStyle;///����
	FLOAT LineWidth;///�߿�
	INT FillStyle;///�����ʽ
	FLOAT FontSize;///�����С
	INT FontStyle; ///������   λ��  Bold=1 Italic=2 Underline=4 Strikeout=4
	INT TextAlignment;///���ֶ��뷽ʽ
	INT LineAlignment;///�����ж��뷽ʽ
	FLOAT Transparent;///͸����
	CString FontName; ///��������
	Gdiplus::Color	LineColor;///�߿���ɫ
	Gdiplus::Color  FillColor;///�����ɫ
	Gdiplus::Color FontColor; ///������ɫ
	Gdiplus::Color ActiveColor;///������ɫ
	Gdiplus::Color TraceColor;
	bool ShowBorder;  ///��ʾ�߿�
	bool ShowFill;   ///���ͼ��
	bool ShowTrack;  ///��ʾ������
	bool ShowActive; ///��ʾΪ����
	bool VerticalText; ///��������
	Appearance();
	Appearance(const Appearance& another);
};