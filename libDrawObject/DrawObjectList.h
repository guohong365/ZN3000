#pragma once
#include "DrawObject.h"

class CDrawObjectList:public CObject
{
private:
	CObjectsList _objectList;       //CTypedPtrList<CObList, CDrawObject *>
	DECLARE_SERIAL(CDrawObjectList);
public:
	CDrawObjectList();

	virtual ~CDrawObjectList();
	BOOL IsEmpty();
	POSITION GetHeadPosition();
	CDrawObject * GetNext(POSITION & pos);
	void RemoveAt(POSITION pos);
	void AddHead(CDrawObject* pObject);
	void AddTail(CDrawObject* pObject);
	void RemoveAll();
	INT_PTR GetCount();
	CDrawObject * GetAt(POSITION pos);
	CDrawObject * GetHead();
	CDrawObject *GetTail();
	CDrawObject* RemoveHead();
	CDrawObject* RemoveTail();
	POSITION GetTailPosition();
	CDrawObject* GetPrev(POSITION &pos);
	POSITION Find(CDrawObject *pItem);

    const CDrawObject* GetAt(POSITION position) const;
    void SetAt(POSITION pos, CDrawObject* newElement);

    // inserting before or after a given position
    POSITION InsertBefore(POSITION position, CDrawObject* newElement);
    POSITION InsertAfter(POSITION position, CDrawObject* newElement);
    POSITION FindIndex(INT_PTR nIndex) const;


	void MoveToHead(CDrawObject * pItem);
	void MoveToTail(CDrawObject * pItem);
	void MoveToPrevior(CDrawObject * pItem);
	void MoveToNext(CDrawObject * pItem);

	void Draw(Gdiplus::Graphics & graph);

	CDrawObject *GetObjectAt(Gdiplus::Point point);
	bool GetObjectInRect(const Gdiplus::Rect & rect, CDrawObjectList * objList);

	CDrawObject *SelectAt(Gdiplus::Point pt, CDrawObjectList * pObjList=NULL);
	void Deselect(CDrawObject*pObject, CDrawObjectList *pObjList=NULL);
	bool SelectInRect(const Gdiplus::Rect & rc, CDrawObjectList * objList=NULL);
	int SelectAll(CDrawObjectList * pObjList = NULL);
	void DeselectAll(CDrawObjectList *pObjList=NULL);

	virtual void Serialize(CArchive & ar);

};
