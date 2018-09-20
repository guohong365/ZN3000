#include "stdafx.h"
#include "DrawObject.h"
#include "History.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CHistory::CHistory( LPCTSTR lpszActionName, CDrawObject * pChanged, CHistory * pNext ) 
:_strActionName(lpszActionName)
,_pChanged(pChanged)
,_pNext(pNext)
{

}

const CString & CHistory::getName()
{
	return _strActionName;
}

void CHistory::undo()
{
    CHistory *pNext=this;
    while(pNext)
    {
        pNext->onUndo(pNext->_pChanged);
        pNext=pNext->_pNext;
    }
}

void CHistory::redo()
{
    CHistory *pNext=this;
    while(pNext)
    {
        pNext->onRedo(pNext->_pChanged);
        pNext=pNext->_pNext;
    }
}

CHistory *CHistory::optimize()
{
	return onOptimaize();
}

CHistory::~CHistory()
{

}

const CString CHistory::getAttributeName( UINT index )
{
	CString name;
	name.LoadString(index);
	return name;
}

Gdiplus::Image * CHistory::QueryImage()
{
	return onQueryImage();
}

Gdiplus::Image * CHistory::onQueryImage()
{
	return NULL;
}

CHistory * CHistory::onOptimaize()
{
    return this;
}

CHistory * CHistory::next()
{
	return _pNext;
}

void CHistory::releaseUndo()
{

}

void CHistory::releaseRedo()
{

}

CDrawObject * CHistory::changed()
{
    return _pChanged;
}

CHistoryAddObject::CHistoryAddObject( CDrawObject * pContainer, CDrawObject * pAdded, CDrawObject * pAfter, CHistory *pNext)
:CHistory(_T("Ìí¼Ó"), pContainer, pNext)
,_pAdded(pAdded)
,_pAfter(pAfter)
{
    int a=0;
}

void CHistoryAddObject::releaseRedo()
{
    delete _pAdded;
}

void CHistoryAddObject::onUndo( CDrawObject *responsor )
{
    responsor->notifyChildDeleted(_pChanged, _pAdded, _pAfter);
}

void CHistoryAddObject::onRedo( CDrawObject *responsor )
{
    responsor->notifyChildAdded(_pChanged, _pAdded, _pAfter);
}

CHistory * CHistoryAddObject::onOptimaize()
{
    return this;
}

CHistoryDeleteObject::CHistoryDeleteObject( CDrawObject * pContainer, CDrawObject * pDeleted, CDrawObject *pAfter, CHistory * pNext ) :CHistory(_T("É¾³ý"), pContainer, pNext)
,_pDeleted(pDeleted)
,_pAfter(pAfter)
{

}

CHistoryDeleteObject::~CHistoryDeleteObject()
{

}

void CHistoryDeleteObject::releaseUndo()
{
    delete _pDeleted;
}

void CHistoryDeleteObject::onUndo( CDrawObject *responsor )
{
    responsor->notifyChildAdded(_pChanged, _pDeleted, _pAfter);
}

void CHistoryDeleteObject::onRedo( CDrawObject *responsor )
{
    responsor->notifyChildDeleted(_pChanged, _pDeleted, _pAfter);
}


void CHistoryAttributeChanged::onUndo( CDrawObject *responsor )
{
    responsor->notifyAttribtesChanged(_pChanged,_attributeIndex, _oldVal, _newVal);
}

void CHistoryAttributeChanged::onRedo( CDrawObject *responsor )
{
    responsor->notifyAttribtesChanged(_pChanged, _attributeIndex, _newVal, _oldVal);
}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex,INT32 newVal,const INT32 oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex,INT64 newVal,INT64 oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex,float newVal, float oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex,double newVal,double oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex,bool newVal,bool oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex, Gdiplus::Size newVal, Gdiplus::Size oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex, Gdiplus::Point newVal,Gdiplus::Point oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex, Gdiplus::Rect newVal,Gdiplus::Rect oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex,Gdiplus::Image * newVal, Gdiplus::Image * oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex, const CString & newVal, const CString & oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

CHistoryAttributeChanged::CHistoryAttributeChanged( CDrawObject *pChanged, UINT attributeIndex, Gdiplus::ColorMatrix & newVal, Gdiplus::ColorMatrix & oldVal, CHistory * pNext ) :CHistory(_T(""), pChanged, pNext)
,_newVal(newVal)
,_oldVal(oldVal)
,_attributeIndex(attributeIndex)
{

}

void CHistoryAttributeChanged::releaseRedo()
{
    _newVal.finalRelease();
}

void CHistoryAttributeChanged::releaseUndo()
{
    _oldVal.Release();
}

CHistory * CHistoryAttributeChanged::onOptimaize()
{
    return this;
    if(_newVal._type!=VALUE_IMAGE)
    {
        CHistoryAttributeChanged *pNext=dynamic_cast<CHistoryAttributeChanged*>(next());
        if(pNext && pNext->changed() ==changed() &&
            pNext->_attributeIndex== _attributeIndex)
        {
            CHistory *pRemain=pNext->next();
            _oldVal=pNext->_oldVal;
            pNext->releaseRedo();
            pNext->releaseUndo();
            delete pNext;
            _pNext=pRemain;
        }
    }
    return this;
}

CHistoryOrderChanged::CHistoryOrderChanged( CDrawObject * pContainer, CDrawObject* pChild, CDrawObject * pNewAfter, CDrawObject * pOldAfter, CHistory * pNext ) :CHistory(_T("Ë³Ðò"), pContainer, pNext)
,_pChild(pChild)
,_pNewAfter(pNewAfter)
,_pOldAfter(pOldAfter)
{

}

void CHistoryOrderChanged::onUndo( CDrawObject *responsor )
{
    responsor->notifyOrderChanged(_pChanged, _pChild, _pOldAfter, _pNewAfter);
}

void CHistoryOrderChanged::onRedo( CDrawObject *responsor )
{
    responsor->notifyOrderChanged(_pChanged, _pChild, _pNewAfter, _pOldAfter);
}

CHistoryDataGroup::CHistoryDataGroup( const CString & name) :CHistory(name, NULL, NULL)
{

}

CHistoryDataGroup::~CHistoryDataGroup()
{
    std::for_each(_undoGroup.begin(), _undoGroup.end(), release_history());
    _undoGroup.clear();
}

void CHistoryDataGroup::addAction( CHistory * pUndoData )
{
    _undoGroup.insert(_undoGroup.begin(),pUndoData);
}

void CHistoryDataGroup::onUndo( CDrawObject *responsor )
{
    std::for_each(_undoGroup.begin(), _undoGroup.end(), group_undo_class());
}

void CHistoryDataGroup::onRedo( CDrawObject *responsor )
{
    std::for_each(_undoGroup.rbegin(), _undoGroup.rend(), group_redo_class());
}

void CHistoryDataGroup::releaseRedo()
{
    std::for_each(_undoGroup.begin(), _undoGroup.end(), release_redo_history());
    _undoGroup.clear();
}

void CHistoryDataGroup::releaseUndo()
{
    std::for_each(_undoGroup.begin(), _undoGroup.end(), release_undo_history());
    _undoGroup.clear();
}

CHistoryRevertable::CHistoryRevertable( const CString & name, CDrawObject *pChanged, int action, CHistory *pNext ) :CHistory(name, pChanged, pNext)
,_actionId(action)
{

}

void CHistoryRevertable::onUndo( CDrawObject *responsor )
{
    responsor->notifyRevert(_pChanged, _actionId);
}

void CHistoryRevertable::onRedo( CDrawObject *responsor )
{
    responsor->notifyRevert(_pChanged, _actionId);
}
