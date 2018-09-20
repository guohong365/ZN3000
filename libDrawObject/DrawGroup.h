#pragma once
#include "drawobject.h"
#include "DrawObjectList.h"

class CDrawGroup :
	public CDrawObject
{
protected:
	DECLARE_SERIAL(CDrawGroup)
	CDrawGroup(void);
public:
	virtual ~CDrawGroup(void);

	static CDrawGroup* CreateGroup(CDrawObjectList * pObjects, CDrawObject * pParent=NULL);
	CDrawGroup & AddToGroup(CDrawObject *pObject);
	CDrawGroup & CopyToGroup(CDrawObject *pObject);
	void Ungroup(CDrawObject *newParent=NULL);
	void UngroupAll(CDrawObject *newParent=NULL);
	CDrawObject * FindObject(const CString & name);
	CDrawGroup * RemoveFromGroup(CDrawObject *pObject);
	CDrawObject * GetObjectAt(const Gdiplus::Point &point);

	virtual void OnSizeChanged();
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);
	virtual void OnDrawBorder(Gdiplus::Graphics & graph);
	virtual void OnDraw(Gdiplus::Graphics & graph);
	virtual Gdiplus::Rect GetBounds();
private:
	CDrawObjectList _objects;
};
