#include "stdafx.h"
#include <Rpc.h>
#include "DrawObject.h"
#include "CustomItems.h"
#include "DrawObjectList.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_SERIAL(CDrawObjectList, CDrawObject, 0)
//////////////////////////////////////////////////////////////////////////

void CDrawObjectList::MoveToHead(CDrawObject * pItem)
{
    POSITION pos = _objectList.Find(reinterpret_cast < CObject * >(pItem));
    if (pos && pos != _objectList.GetHeadPosition())
    {
        _objectList.RemoveAt(pos);
        _objectList.AddHead(pItem);
    }
}

void CDrawObjectList::MoveToTail(CDrawObject * pItem)
{
    POSITION pos = _objectList.Find(reinterpret_cast < CObject * >(pItem));
    if (pos && pos != _objectList.GetTailPosition())
    {
        _objectList.RemoveAt(pos);
        _objectList.AddTail(pItem);
    }
}

void CDrawObjectList::MoveToPrevior(CDrawObject * pItem)
{
    if (!pItem)
    {
        return;
    }
    POSITION pos = _objectList.Find(pItem);
	if(pos && pos!= _objectList.GetHeadPosition())
	{
		POSITION posPrev = pos;
		_objectList.GetPrev(posPrev);
		_objectList.RemoveAt(pos);
		_objectList.InsertBefore(posPrev, pItem);
	}
}

void CDrawObjectList::MoveToNext(CDrawObject * pItem)
{
    if (!pItem)
    {
        return;
    }
    POSITION pos =_objectList.Find(pItem);
	if (pos != _objectList.GetTailPosition())
	{
		POSITION posNext = pos;
		_objectList.GetNext(posNext);
		_objectList.RemoveAt(pos);
		_objectList.InsertAfter(posNext, pItem);
	}

}

void CDrawObjectList::Draw(Gdiplus::Graphics & graph)
{
    POSITION pos = _objectList.GetHeadPosition();
    while (pos)
    {
        CDrawObject *pObject = _objectList.GetNext(pos);
        ASSERT(pObject);
        pObject->Draw(graph);
    }
}

CDrawObject *CDrawObjectList::GetObjectAt(Gdiplus::Point point)
{
    POSITION pos = _objectList.GetTailPosition();
    while (pos)
    {
        CDrawObject *pObject = _objectList.GetPrev(pos);
        if (pObject->HitTest(point))
        {
            return pObject;
        }
    }
    return NULL;
}

bool CDrawObjectList::GetObjectInRect(const Gdiplus::Rect & rect, CDrawObjectList * objList)
{
    return true;
}

CDrawObject *CDrawObjectList::SelectAt(Gdiplus::Point pt, CDrawObjectList * pObjList)
{
    CDrawObject *pObject = GetObjectAt(pt);
    if (pObject)
    {
        pObject->SetSelected(true);
        if (pObjList && !pObjList->Find(pObject))
        {
            pObjList->AddHead(pObject);
        }
    }
    return pObject;
}

bool CDrawObjectList::SelectInRect(const Gdiplus::Rect & rc, CDrawObjectList * objList)
{
    return true;
}

int CDrawObjectList::SelectAll(CDrawObjectList * pObjList /*=NULL*/ )
{
    POSITION pos = GetHeadPosition();
    CDrawObject *pObject;
    while (pos)
    {
        pObject = GetNext(pos);
        pObject->SetSelected(true);
        if (pObjList && pObjList->Find(pObject))
        {
            pObjList->AddHead(pObject);
        }
    }
    return (int) GetCount();
}

CDrawObjectList::CDrawObjectList()
{

}

CDrawObjectList::~CDrawObjectList()
{

}

BOOL CDrawObjectList::IsEmpty()
{
	return _objectList.IsEmpty();
}

POSITION CDrawObjectList::GetHeadPosition()
{
	return _objectList.GetHeadPosition();
}

CDrawObject * CDrawObjectList::GetNext( POSITION & pos )
{
	return _objectList.GetNext(pos);
}

void CDrawObjectList::RemoveAt( POSITION pos )
{
	_objectList.RemoveAt(pos);
}

void CDrawObjectList::AddHead( CDrawObject* pObject )
{
	ASSERT(pObject);
	_objectList.AddHead(pObject);
}

void CDrawObjectList::AddTail( CDrawObject* pObject )
{
	ASSERT(pObject);
	_objectList.AddTail(pObject);
}

void CDrawObjectList::RemoveAll()
{
	_objectList.RemoveAll();
}

INT_PTR CDrawObjectList::GetCount()
{
	return _objectList.GetCount();
}


CDrawObject* CDrawObjectList::GetAt( POSITION position )
{
    return _objectList.GetAt(position);
}

const CDrawObject* CDrawObjectList::GetAt( POSITION position ) const
{
    return _objectList.GetAt(position);
}

CDrawObject * CDrawObjectList::GetHead()
{
	return _objectList.GetHead();
}

CDrawObject * CDrawObjectList::GetTail()
{
	return _objectList.GetTail();
}

CDrawObject* CDrawObjectList::RemoveHead()
{
	return _objectList.RemoveHead();
}

CDrawObject* CDrawObjectList::RemoveTail()
{
	return _objectList.RemoveTail();
}

POSITION CDrawObjectList::GetTailPosition()
{
	return _objectList.GetTailPosition();
}

CDrawObject* CDrawObjectList::GetPrev( POSITION &pos )
{
	return _objectList.GetPrev(pos);
}

POSITION CDrawObjectList::Find( CDrawObject *pItem )
{
	return _objectList.Find(pItem);
}

void CDrawObjectList::Deselect( CDrawObject*pObject, CDrawObjectList *pObjList/*=NULL*/ )
{
	POSITION pos=_objectList.Find(pObject);
	if(pos)  //just deselect owned object
	{
		pObject->SetSelected(false);
		if(pObjList)
		{
			pObjList->RemoveAt(pos);
		}
	}
}

void CDrawObjectList::DeselectAll( CDrawObjectList *pObjList/*=NULL*/ )
{
	POSITION pos=_objectList.GetHeadPosition();
	while(pos)
	{
		CDrawObject *pObject=_objectList.GetNext(pos);
		ASSERT(pObject);
		pObject->SetSelected(false);
		if(pObjList)
		{
			POSITION del=pObjList->Find(pObject);
			if(del)
			{
				pObjList->RemoveAt(pos);
			}
		}
	}
}

void CDrawObjectList::Serialize( CArchive & ar )
{
	int count;
	if(ar.IsLoading())
	{
		ar >> count;
		for(int i=0; i< count; i++)
		{
			CDrawObject *pObject;
			ar>> pObject;
			_objectList.AddTail(pObject);
		}
	}
	else
	{
		count=_objectList.GetCount();
		ar << count;
		POSITION pos=GetHeadPosition();
		while(pos)
		{
			CDrawObject *pObjcet=GetNext(pos);
			ar << pObjcet;
		}
	}
}

void CDrawObjectList::SetAt( POSITION pos, CDrawObject* newElement )
{
    _objectList.SetAt(pos, newElement);
}

POSITION CDrawObjectList::InsertBefore( POSITION position, CDrawObject* newElement )
{
    return _objectList.InsertBefore(position, newElement);
}

POSITION CDrawObjectList::InsertAfter( POSITION position, CDrawObject* newElement )
{
    return _objectList.InsertAfter(position, newElement);
}

POSITION CDrawObjectList::FindIndex( INT_PTR nIndex ) const
{
    return _objectList.FindIndex(nIndex);
}
