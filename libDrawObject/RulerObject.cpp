// Layout.cpp : 实现文件
//

#include "stdafx.h"
#include "RulerObject.h"
#include <tchar.h>
#include <math.h>
#include "MathUtility.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct UNIT_NAME
{
    TCHAR *enName;
    TCHAR *zhName;
};

static UNIT_NAME UnitName[] = {
    {_T("mm"), _T("毫米")},
    {_T("cm"), _T("厘米")},
    {_T("dm"), _T("分米")},
    {_T("m"), _T("米")},
    {_T("km"), _T("千米")},
    {NULL, NULL}
};
static double UnitTrasferMatrix[5][5] = {
    // mm        cm        dm        m         km
    {1.0, 10.0, 100.0, 1000.0, 1000000.0},      //mm
    {0.1, 1.0, 10.0, 100.0, 100000.0},  //cm
    {0.01, 0.1, 1.0, 10.0, 10000.0},    //dm
    {0.001, 0.01, 0.1, 1.0, 1000.0},    //m
    {0.000001, 0.00001, 0.0001, 0.001, 1.0}     //km
};

static TCHAR *get_UnitNameEn(int unit)
{
    if (unit > 5)
    {
        return _T("unknown");
    }
    return UnitName[unit].enName;
}
static TCHAR *get_UnitNameZh(int unit)
{
    if (unit > 5)
    {
        return _T("未知");
    }
    return UnitName[unit].zhName;
}
static int get_UnitByEnName(const TCHAR * name)
{
    int i = 0;
    while (UnitName[i].enName != NULL)
    {
        if (_tcsicmp(UnitName[i].enName, name) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}
static int get_UnitByZhName(const TCHAR * name)
{
    int i = 0;
    while (UnitName[i].zhName != NULL)
    {
        if (_tcsicmp(UnitName[i].zhName, name) == 0)
        {
            return i;
        }
        i++;
    }
    return -1;
}

using namespace Gdiplus;

IMPLEMENT_SERIAL(CRulerObject, CRectObject, 0)
void CRulerObject::Initialize()
{
    m_CurrentUnit = RulerUnitMillimeter;
    m_UnitPerPixel = 0.0;
    m_Align = AlignLeftTop;
    m_GraduationFactor = 1;
    m_GraduationPosition = BaselineAtTop;
    m_LongGraduationInterval = 50;
    m_bShowNumber = false;
    m_IncreaseDirection = IncreaseNormally;
    m_MinNumberPixelInterval = 5;
    m_NumberPosition = NumAtCenter;
    m_NumberFactor = 5;
    m_bShowUnitName = false;
    m_GraduationPixelInterval = 0;
    m_GraduationUnitInterval = 0;
    m_NumberPixelInterval = 0;
    m_NumberUnitInterval = 0;
    SetRotateCenter(Gdiplus::Point(0, 0));
}

CRulerObject::CRulerObject()
{
    Initialize();
}

CRulerObject::CRulerObject(const CString & name, const Gdiplus::Point & point, const Gdiplus::Size & size):CRectObject(name, point, size)
{
    Initialize();
}
CRulerObject::CRulerObject(const CString & name, const Gdiplus::Rect & rect):CRectObject(name, rect)
{
    Initialize();
}
CRulerObject::CRulerObject(const CString & name, int x, int y, int width, int height):CRectObject(name, x, y, width, height)
{
    Initialize();
}


//取英文单位名
const TCHAR *CRulerObject::GetUnitEnName(long unit)
{
    return get_UnitNameEn(unit);
}

//取中文单位名
const TCHAR *CRulerObject::GetUnitZhName(long unit)
{
    return get_UnitNameZh(unit);
}

//从英文名称取单位代码
long CRulerObject::GetUnitByEnName(const TCHAR * unitName)
{
    return get_UnitByEnName(unitName);
}

//从中文名称取单位代码
long CRulerObject::GetUnitByZhName(const TCHAR * unitName)
{
    return get_UnitByZhName(unitName);
}


//////////////////////////////////////////////////////////////////////////
//几何参数

//当前单位，mm,cm等
void CRulerObject::SetCurrentUnit(long unit)
{
    if (m_CurrentUnit != unit)
    {
        OnCurrentUnitChanging(unit);
        m_CurrentUnit = unit;
        OnCurrentUnitCHanged();
    }
}
long CRulerObject::GetCurrentUnit() const
{
    return m_CurrentUnit;
}
void CRulerObject::OnCurrentUnitChanging(long &newUnit)
{
	Notify(DNM_RULER_UNIT_CHANGING, 0, &newUnit);
}
void CRulerObject::OnCurrentUnitCHanged()
{
	Notify(DNM_RULER_UNIT_CHANGED);
}

//每像素的长度
void CRulerObject::SetUnitPerPixel(double unitPerPixel)
{
    if (m_UnitPerPixel != unitPerPixel)
    {
        OnUnitPerPixelChanging(unitPerPixel);
        m_UnitPerPixel = unitPerPixel;
        OnUnitPerPixelChanged();
    }
}
double CRulerObject::GetUnitPerPixel() const
{
    return m_UnitPerPixel;
}
void CRulerObject::OnUnitPerPixelChanging(double &newUnitPerPixel)
{
	Notify(DNM_RULER_SCALE_CHANGING, 0, &newUnitPerPixel);
}
void CRulerObject::OnUnitPerPixelChanged()
{
	Notify(DNM_RULER_SCALE_CHANGED);
}

//标尺对齐方式
void CRulerObject::SetAlignment(long align)
{
    if (m_Align != align)
    {
        OnAlignmentChanging(align);
        m_Align = align;
        OnAlignmentChanged();
    }
}
long CRulerObject::GetAlignment() const
{
    return m_Align;
}
void CRulerObject::OnAlignmentChanging(long &align)
{
	Notify(DNM_RULER_ALIGN_CHANGING, 0, &align);
}
void CRulerObject::OnAlignmentChanged()
{
	Notify(DNM_RULER_ALIGN_CHANGED);
}

//////////////////////////////////////////////////////////////////////////
//刻度线参数

//刻度线最小像素间隔
void CRulerObject::SetMinGraduationPixelInteral(int interval)
{
    if (m_MinGraduationPixelInterval != interval)
    {
        OnMinGraduationPixelIntervalChanging(interval);
        m_MinGraduationPixelInterval = interval;
        OnMinGraduationPixelIntervalChanged();
    }
}
int CRulerObject::GetMinGraduationPixelInterval() const 
{
    return m_MinGraduationPixelInterval;
}
void CRulerObject::OnMinGraduationPixelIntervalChanging(int &newInterval)
{
	Notify(DNM_RULER_MINI_GRADUATION_INTERVAL_CHANGING, 0, &newInterval);
}
void CRulerObject::OnMinGraduationPixelIntervalChanged()
{
	Notify(DNM_RULER_MINI_GRADUATION_INTERVAL_CHANGED);
}

//刻度线单位数字倍数因子。刻度线必须间隔整数倍该因子倍数。
void CRulerObject::SetGraduationFactor(int factor)
{
    if (m_GraduationFactor != factor)
    {
        OnGraduationFactorChanging(factor);
        m_GraduationFactor = factor;
        OnGraduationFactorChanged();
    }
}
int CRulerObject::GetGraduationFactor() const 
{
    return m_GraduationFactor;
}
void CRulerObject::OnGraduationFactorChanging(int &newFactor)
{
    Notify(DNM_RULER_GRADUATION_FACTOR_CHANGING, 0, &newFactor);
}
void CRulerObject::OnGraduationFactorChanged()
{
	Notify(DNM_RULER_GRADUATION_FACTOR_CHANGED);
}

//刻度线相对标尺基线的位置
void CRulerObject::SetGraduationPosition(long position)
{
    if (m_GraduationFactor != position)
    {
        OnGraduationPositionChanging(position);
        m_GraduationPosition = position;
        OnGraduationPositionChanged();
    }
}
long CRulerObject::GetGraduationPosition() const 
{
    return m_GraduationPosition;
}
void CRulerObject::OnGraduationPositionChanging(long &newPos)
{
	Notify(DNM_RULER_GRADUATION_POS_CHANGING, 0, &newPos);
}
void CRulerObject::OnGraduationPositionChanged()
{
	Notify(DNM_RULER_GRADUATION_POS_CHANGED);
}

//长刻度线间隔条数
void CRulerObject::SetLongGraduationInterval(int interval)
{
    if (m_LongGraduationInterval != interval)
    {
        OnLongGraduationIntervalChanging(interval);
        m_LongGraduationInterval = interval;
        OnLongGraduationIntervalChanged();
    }

}
int CRulerObject::GetLongGraduationInterval() const 
{
    return m_LongGraduationInterval;
}
void CRulerObject::OnLongGraduationIntervalChanging(int &interval)
{
	Notify(DNM_RULER_GRADUATION_INTERVAL_CHANGING, 0, &interval);
}
void CRulerObject::OnLongGraduationIntervalChanged()
{
	Notify(DNM_RULER_GRADUATION_INTERVAL_CHANGED);
}

//////////////////////////////////////////////////////////////////////////
//刻度线

//是否显示刻度数字
void CRulerObject::SetShowNumber(bool isShowNumber)
{
    if (m_bShowNumber != isShowNumber)
    {
        OnShowNumberChanging(isShowNumber);
        m_bShowNumber = isShowNumber;
        OnShowNumberChanged();
    }
}
bool CRulerObject::GetShowNumber() const 
{
    return m_bShowNumber;
}

void CRulerObject::OnShowNumberChanging(bool & isShowNumber)
{
    Notify(DNM_RULER_SHOW_NUMBER_CHANGING, 0, &isShowNumber);
}
void CRulerObject::OnShowNumberChanged()  
{
	Notify(DNM_RULER_SHOW_NUMBER_CHANGED);
}

//刻度数字增长方向
void CRulerObject::SetIncreaseDirection(long direction)
{
    if (m_IncreaseDirection != direction)
    {
        OnIncreaseDirectionChanging(direction);
        m_IncreaseDirection = direction;
        OnIncreaseDirectionChanged();
    }
}
long CRulerObject::GetIncreaseDirection() const 
{
    return m_IncreaseDirection;
}
void CRulerObject::OnIncreaseDirectionChanging(long &direction)
{
	Notify(DNM_RULER_INC_DIR_CHANGING, 0, &direction);
}
void CRulerObject::OnIncreaseDirectionChanged()
{
	Notify(DNM_RULER_INC_DIR_CHANGED);
}

//刻度数字间最小像素间隔
void CRulerObject::SetMinNumberPixelInterval(int interval)
{
    if (m_MinNumberPixelInterval != interval)
    {
        MinNumberPixelIntervalChanging(interval);
        m_MinNumberPixelInterval = interval;
        MinNumberPixelIntervalChanged();
    }
}
int CRulerObject::GetMinNumberPixelInterval() const 
{
    return m_MinNumberPixelInterval;
}
void CRulerObject::MinNumberPixelIntervalChanging(int &interval)
{
    Notify(DNM_RULER_NUMBER_INTERVAL_CHANGING, 0, &interval);
}
void CRulerObject::MinNumberPixelIntervalChanged()
{
	Notify(DNM_RULER_NUMBER_INTERVAL_CHANGED);
}

//刻度数字标记位置（相对待标记刻度线）
void CRulerObject::SetNumberPosition(long position)
{
    if (m_NumberPosition != position)
    {
        OnNumberPositionChanging(position);
        m_NumberPosition = position;
        OnNumberPositionChanged();
    }
}
long CRulerObject::GetNumberPosition() const 
{
    return m_NumberPosition;
}
void CRulerObject::OnNumberPositionChanging(long &pos)
{
	Notify(DNM_RULER_NUMBER_POS_CHANGING, 0, &pos);
}
void CRulerObject::OnNumberPositionChanged()
{
	Notify(DNM_RULER_NUMBER_POS_CHANGED);
}

//刻度数字倍数因子，刻度标记数字为该因子整数倍
void CRulerObject::SetNumberFactor(int factor)
{
    if (m_NumberFactor != factor)
    {
        OnNumberFactorChanging(factor);
        m_NumberFactor = factor;
        OnNumberFactorChanged();
    }
}
int CRulerObject::GetNumberFactor() const 
{
    return m_NumberFactor;
}
void CRulerObject::OnNumberFactorChanging(int &factor)
{
	Notify(DNM_RULER_NUMBER_FACTOR_CHANGING, 0, &factor);

}
void CRulerObject::OnNumberFactorChanged()
{
	Notify(DNM_RULER_NUMBER_FACTOR_CHANGED);
}

//////////////////////////////////////////////////////////////////////////
//标尺单位

//是否显示刻度单位
void CRulerObject::SetShowUnitName(bool isShowUnitName)
{
    if (m_bShowUnitName != isShowUnitName)
    {
        OnShowUnitNameChanging(isShowUnitName);
        m_bShowUnitName = isShowUnitName;
        OnShowUnitNameChanged();
    }
}
bool CRulerObject::GetShowUnitName() const 
{
    return m_bShowUnitName;
}

void CRulerObject::OnShowUnitNameChanging(bool & isShowUnitName)
{
    Notify(DNM_RULER_UNIT_NAME_CHANGING, 0, &isShowUnitName);
}
void CRulerObject::OnShowUnitNameChanged()
{
	Notify(DNM_RULER_UNIT_NAME_CHANGED);
}

//////////////////////////////////////////////////////////////////////////
//刻度线间隔控制参数

//刻度线间隔像素
double CRulerObject::GetGraduationPixelInterval() const 
{
    return m_GraduationPixelInterval;
}

//刻度线间隔长度
double CRulerObject::GetGraduationUnitInterval() const 
{
    return m_GraduationUnitInterval;
}

//////////////////////////////////////////////////////////////////////////
//刻度数字标记控制参数

//数字标记间隔像素
int CRulerObject::GetNumberPixelInterval() const 
{
    return m_NumberPixelInterval;
}

//数字标记间隔长度
int CRulerObject::GetNumberUnitInterval() const 
{
    return m_NumberUnitInterval;
}

//////////////////////////////////////////////////////////////////////////
//复制函数
CDrawObject *CRulerObject::CopyFrom(CDrawObject * source)
{
    ASSERT(source);
    CDrawObject::CopyFrom(source);

    CRulerObject *pRulerObject = DYNAMIC_DOWNCAST(CRulerObject, source);
    ASSERT(pRulerObject);

    m_CurrentUnit = pRulerObject->m_CurrentUnit;
    m_UnitPerPixel = pRulerObject->m_UnitPerPixel;
    m_Align = pRulerObject->m_Align;
    m_MinGraduationPixelInterval = pRulerObject->m_MinGraduationPixelInterval;
    m_GraduationFactor = pRulerObject->m_GraduationFactor;
    m_GraduationPosition = pRulerObject->m_GraduationPosition;
    m_LongGraduationInterval = pRulerObject->m_LongGraduationInterval;
    m_bShowNumber = pRulerObject->m_bShowNumber;
    m_IncreaseDirection = pRulerObject->m_IncreaseDirection;
    ;
    m_MinNumberPixelInterval = pRulerObject->m_MinNumberPixelInterval;
    m_NumberPosition = pRulerObject->m_NumberPosition;
    m_NumberFactor = pRulerObject->m_NumberFactor;
    m_bShowUnitName = pRulerObject->m_bShowUnitName;
    m_GraduationPixelInterval = pRulerObject->m_GraduationPixelInterval;
    m_GraduationUnitInterval = pRulerObject->m_GraduationUnitInterval;
    m_NumberPixelInterval = pRulerObject->m_NumberPixelInterval;
    m_NumberUnitInterval = pRulerObject->m_NumberUnitInterval;
    return this;
}


//////////////////////////////////////////////////////////////////////////
//绘图函数
void CRulerObject::OnDraw(Gdiplus::Graphics & graph)
{
    if (m_UnitPerPixel > 0)
    {
		DrawFillObject(graph);
        CalculateGraduatoinStep();
        if (m_GraduationPixelInterval != 0)
        {
            DrawBaseLine(graph);
            DrawGraduation(graph);
            DrawUnitName(graph);
        }
    }
}

//int CRulerObject::HitTest(const Gdiplus::Point &pt)
//{
//      return 0;
//}


//计算刻度线间隔：长度间隔和像素间隔
void CRulerObject::CalculateGraduatoinStep()
{
    double var;
    double totalUnit = m_UnitPerPixel * GetSize().Width;        //总长度
    var = log10(totalUnit);

    int power = (int) var;      //取得指数  

    int divide = (int) (pow(10.0, power) / 10.0);       //将刻度转为1-10之间数的被除数

    //计算最小间隔像素代表的单位数
    var = m_GraduationUnitInterval = divide;
    m_GraduationPixelInterval = var / m_UnitPerPixel;
    while (m_GraduationPixelInterval < m_LongGraduationInterval)        // m_MinNumberPixelInterval)
    {
        var += divide;
        m_GraduationPixelInterval = var / m_UnitPerPixel;
    }
    var = m_GraduationPixelInterval / m_MinNumberPixelInterval;
    if (var >= 10)
    {
        m_GraduationPixelInterval /= 10;
        m_GraduationFactor = 10;
    }
    else if (var >= 5)
    {
        m_GraduationPixelInterval /= 5;
        m_GraduationFactor = 5;
    }
}

//////////////////////////////////////////////////////////////////////////
//绘制基线
void CRulerObject::DrawBaseLine(Gdiplus::Graphics & graph)
{
    Gdiplus::Pen pen(GetLineColor());
    int y;
    if (m_GraduationPosition == BaselineAtTop)
    {
        y = 0;
    }
    else
    {
        y = GetSize().Height;
    }
    graph.DrawLine(&pen, 0, y, (int) ((GetSize().Width) / m_GraduationPixelInterval * m_GraduationPixelInterval), y);
}

//////////////////////////////////////////////////////////////////////////
//绘制标尺刻度线及数字标记
void CRulerObject::DrawGraduation(Gdiplus::Graphics & graph)    //, int unitInterval, int pixelInterval)
{
    int y0, y1;
    double x;
    CString unName;
    Gdiplus::PointF txtPos;
    if (m_GraduationUnitInterval < 10)
    {
        unName.Format(_T("%s"), GetUnitZhName(m_CurrentUnit));
    }
    else
    {
        unName.Format(_T("x%d%s"), (int) m_GraduationUnitInterval, GetUnitZhName(m_CurrentUnit));
    }
    Gdiplus::Size unitSize = GetTextExtent(graph, GetSize().Height, unName);

    Gdiplus::Pen pen(GetLineColor());
    Gdiplus::FontFamily fontFamily(L"宋体");
    Gdiplus::Font font(&fontFamily, (Gdiplus::REAL) (GetSize().Height > 8 ? GetSize().Height : 8), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(GetLineColor());
    CString num;
    if (m_GraduationPosition == BaselineAtTop)
    {
        y0 = 0;
        y1 = GetSize().Height / 3;
    }
    else
    {
        y0 = GetSize().Height;
        y1 = GetSize().Height * 2 / 3;
    }
    double value = 0;
    double delta = m_GraduationPixelInterval * m_UnitPerPixel;
    int i = 0;
    for (x = 0; x <= GetSize().Width; x += m_GraduationPixelInterval)
    {
        num.Format(_T("%d"), (int) (value / m_GraduationUnitInterval + 0.5));
        Gdiplus::Size numSize = GetTextExtent(graph, GetSize().Height, num);
        if (i % m_GraduationFactor == 0 && i > 0)
        {
            txtPos.X = (Gdiplus::REAL) x;
            if (m_GraduationPosition == BaselineAtTop)
            {
                graph.DrawLine(&pen, (int) x, y0, (int) x, y1 * 2);
                txtPos.Y = (Gdiplus::REAL) (y1 * 2);
            }
            else
            {
                graph.DrawLine(&pen, (Gdiplus::REAL) x, (Gdiplus::REAL) y0, (Gdiplus::REAL) x, GetSize().Height / 3.0f);
                txtPos.Y = GetSize().Height / 3.0f - numSize.Height;
            }

            if (x < GetSize().Width - unitSize.Width - numSize.Width)
            {
                Gdiplus::StringFormat format;
                format.SetAlignment(Gdiplus::StringAlignmentCenter);
                graph.DrawString(num, num.GetLength(), &font, txtPos, &format, &brush);
            }
        }
        else
        {
            graph.DrawLine(&pen, (Gdiplus::REAL) x, (Gdiplus::REAL) y0, (Gdiplus::REAL) x, (Gdiplus::REAL) y1);
        }
        i++;
        value += delta;
    }

}

//////////////////////////////////////////////////////////////////////////
//绘制标尺单位名称
Gdiplus::Size CRulerObject::GetTextExtent(Gdiplus::Graphics & graph, int fontHeight, const CString & text)
{
    HDC hdc = graph.GetHDC();
    HFONT hFont =::CreateFont((fontHeight > 8 ? fontHeight : 8),
                              0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, _T("宋体"));
    HGDIOBJ hObj =::SelectObject(hdc, hFont);
    SIZE txtSize;
    ::GetTextExtentPoint32(hdc, text, text.GetLength(), &txtSize);
    ::SelectObject(hdc, hObj);
    ::DeleteObject(hFont);
    graph.ReleaseHDC(hdc);
    return Gdiplus::Size(txtSize.cx, txtSize.cy);
}

void CRulerObject::DrawUnitName(Gdiplus::Graphics & graph)
{
    CString unitName;
    if (m_GraduationUnitInterval < 10)
    {
        unitName.Format(_T("%s"), GetUnitZhName(m_CurrentUnit));
    }
    else
    {
        unitName.Format(_T("x%d%s"), (int) m_GraduationUnitInterval, GetUnitZhName(m_CurrentUnit));
    }
    Gdiplus::FontFamily fontFamily(L"宋体");
    Gdiplus::Font font(&fontFamily, (Gdiplus::REAL) (GetSize().Height > 8 ? GetSize().Height : 8), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::Size size;
    Gdiplus::PointF point;
    Gdiplus::PointF pointF;
    Gdiplus::RectF recfF;
    Gdiplus::SolidBrush brush(GetLineColor());
    Gdiplus::Size txtSize = GetTextExtent(graph, GetSize().Height, unitName);
    if (m_GraduationPosition == BaselineAtTop)
    {
        if (m_IncreaseDirection == IncreaseNormally)
        {
            point.X = GetSize().Width - txtSize.Width / 2.0f;
        }
        else
        {
            point.X = 0;
        }
        point.Y = txtSize.Height / 2.0f;
    }
    else
    {
        if (m_IncreaseDirection == IncreaseNormally)
        {
            point.X = GetSize().Width - txtSize.Width / 2.0f;
        }
        else
        {
            point.X = 0;
        }
        point.Y = GetSize().Height / 3.0f - txtSize.Height;

    }
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    graph.DrawString(unitName, unitName.GetLength(), &font, point, &format, &brush);
}

void CRulerObject::Serialize(CArchive & ar)
{
    CRectObject::Serialize(ar);
    if (ar.IsStoring())
    {
        ar << m_CurrentUnit;
        ar << m_UnitPerPixel;
        ar << m_Align;
        ar << m_MinGraduationPixelInterval;
        ar << m_GraduationFactor;
        ar << m_GraduationPosition;
        ar << m_LongGraduationInterval;
        ar << m_bShowNumber;
        ar << m_IncreaseDirection;
        ar << m_MinNumberPixelInterval;
        ar << m_NumberPosition;
        ar << m_NumberFactor;
        ar << m_bShowUnitName;
        ar << m_GraduationPixelInterval;
        ar << m_GraduationUnitInterval;
        ar << m_NumberPixelInterval;
        ar << m_NumberUnitInterval;
    }
    else
    {
        ar >> m_CurrentUnit;
        ar >> m_UnitPerPixel;
        ar >> m_Align;
        ar >> m_MinGraduationPixelInterval;
        ar >> m_GraduationFactor;
        ar >> m_GraduationPosition;
        ar >> m_LongGraduationInterval;
        ar >> m_bShowNumber;
        ar >> m_IncreaseDirection;
        ar >> m_MinNumberPixelInterval;
        ar >> m_NumberPosition;
        ar >> m_NumberFactor;
        ar >> m_bShowUnitName;
        ar >> m_GraduationPixelInterval;
        ar >> m_GraduationUnitInterval;
        ar >> m_NumberPixelInterval;
        ar >> m_NumberUnitInterval;
    }
}

void CRulerObject::OnSizeChanging(Gdiplus::Size & newSize)
{
	return;
    CRectObject::OnSizeChanging(newSize);
    ASSERT(newSize.Width);

    if (m_UnitPerPixel != 0.0)
    {
        Gdiplus::Size oldSize = GetSize();
        ASSERT(oldSize.Width);

        double ratio = (double) oldSize.Width / newSize.Width;
        m_UnitPerPixel /= ratio;
    }
}

