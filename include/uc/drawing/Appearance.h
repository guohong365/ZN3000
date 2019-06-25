#pragma once
#include <uc/libuc.h>

namespace uc
{
    namespace drawing{

        struct LIB_UC_API Appearance
        {
            int LineStyle;///线型
            float LineWidth;///线宽
            int FillStyle;///填充样式
            float FontSize;///字体大小
            int FontStyle; ///字体风格   位或  Bold=1 Italic=2 Underline=4 Strikeout=4
            int TextAlignment;///文字对齐方式
            int LineAlignment;///文字行对齐方式
            float Transparent;///透明度
            CString FontName; ///字体名称
            unsigned long LineColor;///线框颜色
            unsigned long   FillColor;///填充颜色
            unsigned long  FontColor; ///字体颜色
            unsigned long  ActiveColor;///激活颜色
            unsigned long  TraceColor;
            bool ShowBorder;  ///显示边框
            bool ShowFill;   ///填充图形
            bool ShowTrack;  ///显示操作柄
            bool ShowActive; ///显示为激活
            bool VerticalText; ///文字竖排
            Appearance();
            Appearance(const Appearance& another);
            virtual ~Appearance(){}
            Appearance& operator=(const Appearance& another);
            virtual void CopyFrom(const Appearance & another);
            virtual Appearance* Clone();
        };

    }
}
