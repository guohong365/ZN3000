#pragma once
#include <GdiPlus.h>
#include "DrawObject.h"
#include "RulerObject.h"
#include "DrawObjectList.h"
#define SELECT_ADD     1
#define SELECT_REPLACE 2

#define HORIZ_RULER 0
#define VERT_RULER  1


class CObjectsContainer:public CRectObject
{
protected:
	CObjectsContainer();
	void Initialize();
	DECLARE_SERIAL(CObjectsContainer) 
public:
	CObjectsContainer(const CString & name, const Gdiplus::Point & pt, const Gdiplus::Size & size, DWORD options =
		WITH_HORIZ_RULER | WITH_VERT_RULER);
	CObjectsContainer(const CString & name, const Gdiplus::Rect & rect, DWORD options = WITH_HORIZ_RULER | WITH_VERT_RULER);
	virtual ~ CObjectsContainer();
	virtual void ResetContent();

	void SetRulerUnit(int nRuler, int unit);
	int GetRulerUnit(int nRuler) const;

	void SetRulerScale(double unitPerPixel);
	double GetRulerScale() const;
	virtual void OnRulerScaleChanging(double &newUnitPerPixel);
	virtual void OnRulerScaleChanged();

	void SetRulerColor(int nRuler, Gdiplus::Color color);
	Gdiplus::Color GetRulerColor(int nRuler) const;
	virtual void OnRulerColorChanging(int nRuler, Gdiplus::Color & color);
	virtual void OnRulerColorChanged(int nRuler);


	void SetShowRuler(int nRuler, bool isShow);
	bool GetShowRuler(int nRuler) const;
	virtual void OnShowRulerChanging(int nRuler, bool & newIsShow);
	virtual void OnShowRulerChanged(int nRuler);

	void SetShowName(bool isShow);
	bool GetShowName() const;
	virtual void OnShowNameChanging(bool & newIsShow);
	virtual void OnShowNameChanged();

	//virtual void Enable(const CString & propertyName);
	//virtual void Disable(const CString & propertyName);
	//virtual bool IsEnabled(const CString & proertyName);

	virtual CDrawObject *CopyFrom(CDrawObject * source);

	virtual void DrawRuler(Gdiplus::Graphics & graph);

	virtual void Serialize(CArchive & ar);

	virtual Gdiplus::Rect GetBounds();


	CDrawObjectList & GetObjects();
	virtual void AddObject(CDrawObject * pObject);
	virtual CDrawObject *RemoveObject(CDrawObject * pObject);

	virtual void Clear();

	virtual void SelectAll(CDrawObjectList & objList, int nFlag = SELECT_ADD);

	virtual CDrawObject *GetObjectsAt(const Gdiplus::Point & point);

	CDrawObject *FindObject(const CString & name);
	virtual CDrawObject *GetTypeOfObjectAt(CRuntimeClass *pClass, const Gdiplus::Point &point);

	//////////////////////////////////////////////////////////////////////////
	virtual bool BuildProperties(CXTPPropertyGridItem * pCategoryObjects);
	virtual void OnPropertyItemChangedNotify(CXTPPropertyGridItem * pItem);
	virtual bool BuildRulerProperty(CXTPPropertyGridItem * pCategoryObjects);
	virtual Gdiplus::Rect GetWorldBounds();
    virtual bool IsEmpty();

protected:
    virtual void notifyAttribtesChanged(CDrawObject *changedObject, unsigned int attributeIndex, ATTR_VALUE & newVal, ATTR_VALUE & oldVal);
    virtual void notifyChildAdded( CDrawObject * container, CDrawObject *childToAdd, CDrawObject *afterObject );
    virtual void notifyChildDeleted( CDrawObject *changedObject, CDrawObject *childToDelete, CDrawObject * afterObject );
    virtual void notifyOrderChanged(CDrawObject* container, CDrawObject * changedObject, CDrawObject * newAfterObject, CDrawObject *oldAfterObject);

	//事件(覆盖)
	virtual void OnDrawTracker(Gdiplus::Graphics & graph);
	virtual void OnSizeChanged();
	virtual void OnDraw(Gdiplus::Graphics & graph);
private:

	double m_RulerUnitPerPixel;

	//////////////////////////////////////////////////////////////////////////
	//绘制控制属性


	//对象选项，1，2位分别控制标迟水平，垂直标尺是否使用
	DWORD m_Options;

	bool m_bShowNameOnBackground;
	//////////////////////////////////////////////////////////////////////////
	//组件属性

	//垂直标尺
	CRulerObject *m_pVerticalRuler;
	//水平标尺
	CRulerObject *m_pHorizalRuler;

	CDrawObjectList m_DrawObjects;
};
