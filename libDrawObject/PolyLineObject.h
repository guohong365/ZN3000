#pragma once
#include "drawobject.h"



class CPolyLineObject:public CDrawObject
{
protected:
	CPolyLineObject(void);
	DECLARE_SERIAL(CPolyLineObject) 
public:
	CPolyLineObject(const CString & name, int x, int y);
	CPolyLineObject(const CString & name, const Gdiplus::Point & pt);
	virtual ~ CPolyLineObject(void);

	void SetVertexes(Gdiplus::Point * pPoints, int Count);
	int GetVertexes(Gdiplus::Point * buffer, int buflen) const;
	virtual void OnVertexBufferChanging(Gdiplus::Point * &pPoints, int &Count);
	virtual void OnVertexBufferChanged();

	int GetVetexCount() const;
	Gdiplus::Point * GetVetexBuffer() const;

	void AddVertex(Gdiplus::Point pt);
	void InsertVertex(int index, Gdiplus::Point pt);
	void RemoveVertex(int index);

	virtual void OnVertexChanging(POLYLINE_NODE_INFO * dnmPolyline);
	virtual void OnVertexChanged(POLYLINE_NODE_INFO *dnmPolyline);

	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDrawFillObject(Gdiplus::Graphics & graph);
	virtual void OnDraw(Gdiplus::Graphics & graph);

	virtual int GetHandleCount();
	virtual Gdiplus::Point GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int &nHandle, Gdiplus::Point point);
	virtual int HitTest(const Gdiplus::Point & pt);

	virtual void Serialize(CArchive & ar);
	virtual CDrawObject *CopyFrom(CDrawObject * source);
	virtual void Scale(double scaleX, double scaleY);
	virtual void ScaleAt(double scaleX, double scaleY, Gdiplus::Point origin);
	virtual Gdiplus::Point GetCenter();
	virtual void GetObjectRegion(Gdiplus::GraphicsPath & region);
	virtual Gdiplus::Rect GetBounds();

	virtual bool GetWorldAngle(double *angle, Gdiplus::Point * rotateCenter);
	virtual Gdiplus::Rect GetWorldBounds();

    virtual void notifyAttribtesChanged(CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal);
private:
	void RecalcSize();
	Gdiplus::Point * m_pPoints;
	int m_PointCount;
	int m_BufferCount;

};
