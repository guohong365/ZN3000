#include "StdAfx.h"
#include "polylineobject.h"
#include "MathUtility.h"
#include "History.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define VERTEX_INCREMENT 10

using namespace Gdiplus;

IMPLEMENT_SERIAL(CPolyLineObject, CDrawObject, 0) 

CPolyLineObject::CPolyLineObject(void)
{
    m_pPoints = new Gdiplus::Point[VERTEX_INCREMENT];
    m_BufferCount = VERTEX_INCREMENT;
    m_PointCount = 0;
}
CPolyLineObject::~CPolyLineObject(void)
{
    delete m_pPoints;
}
CPolyLineObject::CPolyLineObject(const CString & name, int x, int y):CDrawObject(name, x, y, 1, 1)
{
    m_pPoints = new Gdiplus::Point[VERTEX_INCREMENT];
    m_BufferCount = VERTEX_INCREMENT;
    m_PointCount = 0;
    AddVertex(Gdiplus::Point(0, 0));
}
CPolyLineObject::CPolyLineObject(const CString & name, const Gdiplus::Point & pt):CDrawObject(name, pt.X, pt.Y, 1, 1)
{
    m_pPoints = new Gdiplus::Point[VERTEX_INCREMENT];
    m_BufferCount = VERTEX_INCREMENT;
    m_PointCount = 0;
    AddVertex(Gdiplus::Point(0, 0));
}


void CPolyLineObject::SetVertexes(Gdiplus::Point * pPoints, int count)
{
    if (pPoints == m_pPoints)
        return;
    OnVertexBufferChanging(pPoints, count);
    delete m_pPoints;
    m_BufferCount = count + VERTEX_INCREMENT;
    m_pPoints = new Gdiplus::Point[m_BufferCount];
    if (count)
    {
        memcpy(m_pPoints, pPoints, count * sizeof(Gdiplus::Point));
    }
    m_PointCount = count;
    OnVertexBufferChanged();
}
int CPolyLineObject::GetVertexes(Gdiplus::Point * buffer, int count) const 
{
    if (buffer && count)
    {
        if (count > m_PointCount)
        {
            count = m_PointCount;
        }
        if (count)
        {
            memcpy(buffer, m_pPoints, count * sizeof(Gdiplus::Point));
        }
        return count;
    }
    return m_PointCount;
}

void CPolyLineObject::OnVertexBufferChanging(Gdiplus::Point * &pPoints, int &Count)
{

}
void CPolyLineObject::OnVertexBufferChanged()
{

}

int CPolyLineObject::GetVetexCount() const 
{
    return m_PointCount;
}

Gdiplus::Point * CPolyLineObject::GetVetexBuffer() const
{
    return m_pPoints;
}

void CPolyLineObject::AddVertex(Gdiplus::Point pt)
{
    int index = 0;
	POLYLINE_NODE_INFO dnm(POLYLINE_NODE_ADD, m_PointCount,pt);
    OnVertexChanging(&dnm);
    if (m_PointCount == m_BufferCount)
    {
        Gdiplus::Point * newPoints = new Gdiplus::Point[m_BufferCount + VERTEX_INCREMENT];
        if (m_pPoints != NULL)
        {
            memcpy(newPoints, m_pPoints, sizeof(Gdiplus::Point) * m_BufferCount);
            delete[]m_pPoints;
        }
        m_pPoints = newPoints;
        m_BufferCount += VERTEX_INCREMENT;
    }

    m_pPoints[m_PointCount++] = pt;
    OnVertexChanged(&dnm);
}
void CPolyLineObject::InsertVertex(int index, Gdiplus::Point pt)
{
    if (index < 0)              //insert at first
        index = 0;
    if (index > m_PointCount)   //add 
    {
        AddVertex(pt);
        return;
    }
	POLYLINE_NODE_INFO dnm(POLYLINE_NODE_ADD, index, pt);
    OnVertexChanging(&dnm);
    if (m_PointCount + 1 <= m_BufferCount)
    {
        memcpy(m_pPoints + index, m_pPoints + index + 1, sizeof(Gdiplus::Point) * (m_PointCount - index));
        m_pPoints[index] = pt;
        m_PointCount++;
        OnVertexChanged(&dnm);
    }
    else
    {
        Gdiplus::Point * newPoints = new Gdiplus::Point[m_BufferCount + VERTEX_INCREMENT];
        if (m_pPoints != NULL)
        {
            memcpy(newPoints, m_pPoints, sizeof(Gdiplus::Point) * m_BufferCount);
            delete[]m_pPoints;
        }
        m_pPoints = newPoints;
        m_BufferCount += VERTEX_INCREMENT;
        InsertVertex(index, pt);
    }
}

void CPolyLineObject::RemoveVertex(int index)
{
    ASSERT(index >= 0 && index <= m_PointCount - 1);
	POLYLINE_NODE_INFO dnm(POLYLINE_NODE_DELETE,index, GetVetexBuffer()[index]);
    OnVertexChanging(&dnm);
    if (dnm._index == m_PointCount - 1)
    {
        m_PointCount--;
        OnVertexChanged(&dnm);
        return;
    }
    memcpy(m_pPoints + index, m_pPoints + index + 1, sizeof(Gdiplus::Point) * m_PointCount - index - 1);
    m_PointCount--;
    OnVertexChanged(&dnm);
}

void CPolyLineObject::OnVertexChanging(POLYLINE_NODE_INFO * dnmPolyline)
{
	Notify(DNM_VERTEX_CHANGING, 0, &dnmPolyline);
    //switch(action)
    //{
    //case VERTEX_ACTION_ADD:
    //      break;
    //case VERTEX_ACTION_INSERT:
    //      break;
    //case VERTEX_ACTION_REMOVE:
    //      break;
    //}
}
void CPolyLineObject::OnVertexChanged(POLYLINE_NODE_INFO * dnmPolyline)
{
	RecalcSize();
	Notify(DNM_VERTEX_CHANGED, 0, dnmPolyline);
}

void CPolyLineObject::OnDrawBorder(Gdiplus::Graphics & graph)
{
}
void CPolyLineObject::OnDraw(Gdiplus::Graphics & graph)
{
    if (m_PointCount < 2)
        return;
    Gdiplus::Color color = GetLineColor();
    if(GetActive())
    {
        color=GetActiveColor();
    }
    BYTE alpha = (BYTE) (GetTransparent() * 255);
    Gdiplus::ARGB argb = color.GetValue();
    argb &= (((Gdiplus::ARGB) (alpha) << Gdiplus::Color::AlphaShift) | 0x00FFFFFF);
    color = argb;
    Gdiplus::Pen pen(color, (float) GetLineWidth());
	pen.SetDashStyle((DashStyle)GetLineStyle());
    Gdiplus::GraphicsPath path;
    path.AddLines(m_pPoints, m_PointCount);
    graph.DrawPath(&pen, &path);
}

void CPolyLineObject::OnDrawFillObject(Gdiplus::Graphics & graph)
{
}

int CPolyLineObject::GetHandleCount()
{
    return m_PointCount;
}

Gdiplus::Point CPolyLineObject::GetHandle(int nHandle)
{
    return m_pPoints[nHandle - 1];
}

HCURSOR CPolyLineObject::GetHandleCursor(int nHandle)
{
    return AfxGetApp()->LoadStandardCursor(IDC_ARROW);
}
void CPolyLineObject::MoveHandleTo(int &nHandle, Gdiplus::Point point)
{
    ASSERT(nHandle >= 1 && nHandle <= m_PointCount);
    Global2Local(&point);
    m_pPoints[nHandle - 1] = point;
}
int CPolyLineObject::HitTest(const Gdiplus::Point & pt)
{
    int handle = CDrawObject::HitTest(pt);
    if (handle >= 1 && handle <= GetHandleCount())
        return handle;

    Gdiplus::GraphicsPath path;
    path.AddLines(m_pPoints, m_PointCount);
    Gdiplus::Pen pen(Gdiplus::Color::White, 10);
    if (path.IsOutlineVisible(pt, &pen))
    {
        return GetHandleCount() + 1;
    }
    return 0;
}

void CPolyLineObject::Serialize(CArchive & ar)
{
    int i=0;
    CDrawObject::Serialize(ar);
    if (ar.IsLoading())
    {
        ar >> m_PointCount;
		if(m_PointCount >= m_BufferCount)
		{
			m_BufferCount = m_PointCount + VERTEX_INCREMENT;
			delete m_pPoints;
			m_pPoints = new Gdiplus::Point[m_BufferCount];
		}
		
        for (i = 0; i < m_PointCount; i++)
        {
            ar >> m_pPoints[i].X;
            ar >> m_pPoints[i].Y;
        }
    }
    else
    {
        ar << m_PointCount;
        for (i = 0; i < m_PointCount; i++)
        {
            ar << m_pPoints[i].X;
            ar << m_pPoints[i].Y;
        }

    }
}

CDrawObject *CPolyLineObject::CopyFrom(CDrawObject * source)
{
    CPolyLineObject *pObject = DYNAMIC_DOWNCAST(CPolyLineObject, source);
    ASSERT(pObject);
    CDrawObject::CopyFrom(source);
    delete m_pPoints;
    m_BufferCount = pObject->m_BufferCount;
    m_PointCount = pObject->m_PointCount;
    m_pPoints = new Gdiplus::Point[m_BufferCount];
    memcpy(m_pPoints, pObject->m_pPoints, m_PointCount);
    return this;
}

void CPolyLineObject::Scale(double scaleX, double scaleY)
{
	//Gdiplus::Point pos=
    Gdiplus::Point * pt = GetVetexBuffer();
	Gdiplus::Point pos=GetPosition();
	Local2Global(pt, GetVetexCount());

    for (int i = 0; i < GetVetexCount(); i++)
    {
        //pt[i].X =pos.X + (int) ((pt[i].X-pos.X) * scaleX);
        //pt[i].Y =pos.Y + (int) ((pt[i].Y-pos.Y) * scaleY);
		pt[i].X =pos.X + (int) ((pt[i].X-pos.X) * scaleX);
		pt[i].Y =pos.Y + (int) ((pt[i].Y-pos.Y) * scaleY);
    }
	Global2Local(pt, GetVetexCount());
	//Gdiplus::Point center=GetRotateCenter();
	//SetRotateCenter(center);
}


Gdiplus::Point CPolyLineObject::GetCenter()
{
	Gdiplus::Point lt=m_pPoints[0];
	Gdiplus::Point rb=m_pPoints[0];
	for(int i=1; i< m_PointCount; i++)
	{
		if(lt.X > m_pPoints[i].X)
		{
			lt.X=m_pPoints[i].X;
		}
		if(lt.Y>m_pPoints[i].Y)
		{
			lt.Y=m_pPoints[i].Y;
		}
		if(rb.X < m_pPoints[i].X)
		{
			rb.X=m_pPoints[i].X;
		}
		if(rb.Y < m_pPoints[i].Y)
		{
			rb.Y=m_pPoints[i].Y;
		}
	}
	Gdiplus::Point center((rb.X+lt.X)/2, (rb.Y+lt.Y)/2);
	return center;
}

void CPolyLineObject::ScaleAt( double scaleX, double scaleY, Gdiplus::Point origin )
{
	//Gdiplus::Point pos=
	Gdiplus::Point * pt = GetVetexBuffer();
	//Gdiplus::Point pos=GetPosition();
	//Local2Global(pt, GetVetexCount());

	for (int i = 0; i < GetVetexCount(); i++)
	{
		pt[i].X =-origin.X + (int) ((pt[i].X-origin.X) * scaleX);
		pt[i].Y =-origin.Y + (int) ((pt[i].Y-origin.Y) * scaleY);
	}
	//Global2Local(pt, GetVetexCount());
	//Gdiplus::Point center=GetRotateCenter();
	//SetRotateCenter(center);
}

void CPolyLineObject::GetObjectRegion( Gdiplus::GraphicsPath & region )
{
	region.Reset();
	region.AddLines(m_pPoints, m_PointCount);
}

Gdiplus::Rect CPolyLineObject::GetBounds()
{
	Point * pts=new Point[GetVetexCount()];
	GetVertexes(pts, GetVetexCount());
	Local2Global(pts, GetVetexCount());
	int minx=pts[0].X;
	int maxx=pts[0].X;
	int miny=pts[0].Y;
	int maxy=pts[0].Y;
	for(int i=1; i< GetVetexCount(); i++)
	{
		if(minx > pts[i].X)
			minx=pts[i].X;
		if(maxx < pts[i].X)
			maxx=pts[i].X;
		if(miny > pts[i].Y)
			miny=pts[i].Y;
		if(maxy < pts[i].Y)
			maxy=pts[i].Y;
	}
	delete pts;
	return Rect(minx, miny, maxx - minx, maxy-miny);
}

Gdiplus::Rect CPolyLineObject::GetWorldBounds()
{
	Point * pts=new Point[GetVetexCount()];
	GetVertexes(pts, GetVetexCount());
	Local2World(pts, GetVetexCount());
	int minx=pts[0].X;
	int maxx=pts[0].X;
	int miny=pts[0].Y;
	int maxy=pts[0].Y;
	for(int i=1; i< GetVetexCount(); i++)
	{
		if(minx > pts[i].X)
			minx=pts[i].X;
		if(maxx < pts[i].X)
			maxx=pts[i].X;
		if(miny > pts[i].Y)
			miny=pts[i].Y;
		if(maxy < pts[i].Y)
			maxy=pts[i].Y;
	}
	delete pts;
	return Rect(minx, miny, maxx - minx, maxy-miny);
}
void CPolyLineObject::RecalcSize()
{
	return;
	Size size(0,0);
	if(m_PointCount<=1)
	{
		SetSize(size);
		return;
	}
	int minx;
	int miny;
	int maxx;
	int maxy;
	minx=maxx=m_pPoints[0].X;
	miny=maxy=m_pPoints[0].Y;
	for(int i=1; i< m_PointCount; i++)
	{
		if(minx > m_pPoints[i].X)
			minx=m_pPoints[i].X;
		if(maxx < m_pPoints[i].X)
			maxx=m_pPoints[i].X;
		if(miny > m_pPoints[i].Y)
			miny=m_pPoints[i].Y;
		if(maxy < m_pPoints[i].Y)
			maxy=m_pPoints[i].Y;
	}
	size.Width=maxx-minx;
	size.Height=maxy-miny;
	SetSize(size);
}

bool CPolyLineObject::GetWorldAngle(double *angle, Gdiplus::Point * rotateCenter)
{
	ASSERT(GetVetexCount() > 1);
	int i=1;
	do 
	{
		if(GetVetexBuffer()[0].Equals(GetVetexBuffer()[i]))
		{
			i++;
		}
		else
		{
			break;
		}
	} while (i< GetVetexCount());
	if(i>=GetVetexCount())
	{
		return false;
	}
	Point pt1=GetVetexBuffer()[0];
	Point pt2=GetVetexBuffer()[i];
	Local2World(&pt1);
	Local2World(&pt2);
	Point ptCenter;
	World2Local(&ptCenter);
	if(angle)
	{
		*angle=CMathUtility::CalculateLineAngle(pt1.X, pt1.Y, pt2.X, pt2.Y);
	}
	if(rotateCenter)
	{
		*rotateCenter=ptCenter;
	}
	return true;
}

void CPolyLineObject::notifyAttribtesChanged( CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal )
{
    switch(attributeIndex)
    {
    case ID_OBJECT_POLYLINE_NODE:
        {
            POLYLINE_NODE_INFO nodeInfo=newVal;
            switch(nodeInfo._action)
            {
            case POLYLINE_NODE_ADD:
                InsertVertex(nodeInfo._index, nodeInfo._pt);
                break;
            case POLYLINE_NODE_DELETE:
                RemoveVertex(nodeInfo._index);
                break;
            case POLYLINE_NODE_MODIFY:
                GetVetexBuffer()[nodeInfo._index]=nodeInfo._pt;
                break;
            default:
                break;
            }
            break;
        }
    default:
        __super::notifyAttribtesChanged(changedObject, attributeIndex, newVal, oldVal);
        break;
    }
}
