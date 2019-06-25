#pragma once
#include <uc/libuc.h>

namespace uc
{
    namespace drawing{

        struct LIB_UC_API Appearance
        {
            int LineStyle;///����
            float LineWidth;///�߿�
            int FillStyle;///�����ʽ
            float FontSize;///�����С
            int FontStyle; ///������   λ��  Bold=1 Italic=2 Underline=4 Strikeout=4
            int TextAlignment;///���ֶ��뷽ʽ
            int LineAlignment;///�����ж��뷽ʽ
            float Transparent;///͸����
            CString FontName; ///��������
            unsigned long LineColor;///�߿���ɫ
            unsigned long   FillColor;///�����ɫ
            unsigned long  FontColor; ///������ɫ
            unsigned long  ActiveColor;///������ɫ
            unsigned long  TraceColor;
            bool ShowBorder;  ///��ʾ�߿�
            bool ShowFill;   ///���ͼ��
            bool ShowTrack;  ///��ʾ������
            bool ShowActive; ///��ʾΪ����
            bool VerticalText; ///��������
            Appearance();
            Appearance(const Appearance& another);
            virtual ~Appearance(){}
            Appearance& operator=(const Appearance& another);
            virtual void CopyFrom(const Appearance & another);
            virtual Appearance* Clone();
        };

    }
}
