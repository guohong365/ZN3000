#include "StdAfx.h"
#include <DrawObject\UndoManager.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CHistoryManager & CHistoryManager::undoManager()
{
	static CHistoryManager _undoManager;
	return _undoManager;
}

CHistory * CHistoryManager::PopUndoData()
{
	if(_undoStack.empty())
		return NULL;
	CHistory *pUndoData=_undoStack.top();
	_undoStack.pop();
	return pUndoData;
}

CHistory * CHistoryManager::PopRedoData()
{
	if(_redoStack.empty())
	{
		return NULL;
	}
	CHistory *pUndoData=_redoStack.top();
	_redoStack.pop();
	return pUndoData;
}

void CHistoryManager::PushUndoData( CHistory *pUndoData )
{
	_undoStack.push(pUndoData);
}

void CHistoryManager::PushRedoData( CHistory *pUndoData )
{
	_redoStack.push(pUndoData);
}

void CHistoryManager::AddUndo( CHistory *pUndoData )
{
	ClearRedoList();
    if(pUndoData)
    {
        _undoStack.push(pUndoData);
    }
}

void CHistoryManager::ClearUndoList()
{
#if _MSC_VER > 1400
	std::for_each(_undoStack._Get_container().begin(), _undoStack._Get_container().end(), release_undo_history());
#else
	std::for_each(_undoStack.c.begin(), _undoStack.c.end(), release_undo_history());
#endif
    while(_undoStack.size())
        _undoStack.pop();
}

void CHistoryManager::ClearRedoList()
{
#if _MSC_VER > 1400
	std::for_each(_redoStack._Get_container().begin(), _redoStack._Get_container().end(), release_redo_history());
#else
	std::for_each(_redoStack.c.begin(), _redoStack.c.end(), release_redo_history());
#endif
	while(_redoStack.size())
        _redoStack.pop();
}

void CHistoryManager::ClearAll()
{
	ClearRedoList();
	ClearUndoList();
}

bool CHistoryManager::CanUndo()
{
	return !_undoStack.empty();
}

bool CHistoryManager::CanRedo()
{
	return !_redoStack.empty();
}

void CHistoryManager::Undo()
{
	CHistory *pUndoData=PopUndoData();
	if(pUndoData)
	{
		pUndoData->undo();
		PushRedoData(pUndoData);
	}
}

void CHistoryManager::Redo()
{
	CHistory *pUndoData=PopRedoData();
	if(pUndoData)
	{
		pUndoData->redo();
		PushUndoData(pUndoData);
	}
}

CHistoryManager::~CHistoryManager( void )
{
	ClearRedoList();
    ClearUndoList();
    delete _pHistroyLog;
}

CHistroyLog & CHistoryManager::logger()
{
    if(_pHistroyLog==NULL)
    {
        _pHistroyLog=new CHistroyLog(this);
    }
    return *_pHistroyLog;
}

CHistoryManager::CHistoryManager()
:_pHistroyLog(NULL)
{

}

CHistroyLog::CHistroyLog(CHistoryManager * pManager) 
:_log(NULL)
,_inEditing(false)
,_pManager(pManager)
,_inGroupEditing(false)
,_pGroup(NULL)
{

}

bool CHistroyLog::IsInEditing()
{
	return _inEditing;
}

void CHistroyLog::BeginEdit()
{
    Discard();
	ASSERT(!_inEditing);
	_inEditing=true;
}

void CHistroyLog::AddObject( CDrawObject * changed, CDrawObject* added, CDrawObject * after )
{
    ASSERT(changed);
	_log=new CHistoryAddObject(changed, added, after, _log);
    _log=_log->optimize();
}

void CHistroyLog::DeleteObject( CDrawObject * changed, CDrawObject * deleted, CDrawObject * after )
{
    ASSERT(changed);
	_log=new CHistoryDeleteObject(changed, deleted, after, _log);
    _log=_log->optimize();
}


void CHistroyLog::SetOrder( CDrawObject * owner, CDrawObject * changed, CDrawObject * newAfter, CDrawObject * oldAfter )
{
    ASSERT(owner && changed);
	_log=new CHistoryOrderChanged(owner, changed, newAfter, oldAfter, _log);
    _log=_log->optimize();
}

void CHistroyLog::EndEdit()
{
	ASSERT(_inEditing);
    ASSERT(_pManager);
	_inEditing=false;
    _pManager->AddUndo(Detach());
}

CHistory * CHistroyLog::Detach()
{
	ASSERT(!_inEditing);
	CHistory *pRet=_log;
	_log=NULL;
	return pRet;
}

void CHistroyLog::Discard()
{
	release_history release;
	release(_log);
    _inEditing=false;
}

CHistoryManager * CHistroyLog::manager()
{
    return _pManager;
}

void CHistroyLog::BeginGroupEdit( const CString & name )
{
    BeginEdit();
    _inGroupEditing=true;
    _log=_pGroup=new CHistoryDataGroup(name);
}

void CHistroyLog::GroupAddObject( CDrawObject * changed, CDrawObject* added, CDrawObject * after )
{
    ASSERT(changed);

    CHistory *pLog=new CHistoryAddObject(changed, added, after, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupDeleteObject( CDrawObject * changed, CDrawObject * deleted, CDrawObject * after )
{
    ASSERT(changed);
    CHistory *pLog=new CHistoryDeleteObject(changed, deleted, after, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetOrder( CDrawObject * owner, CDrawObject * changed, CDrawObject * newAfter, CDrawObject * oldAfter )
{
    ASSERT(owner && changed);
    CHistory *pLog=new CHistoryOrderChanged(owner, changed, newAfter, oldAfter, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::EndGroupEdit()
{
    ASSERT(_inGroupEditing);
    EndEdit();
    _inGroupEditing=false;
    _pGroup=NULL;
}

void CHistroyLog::SetRevertable(const CString & name, CDrawObject * pObject, int acrionId )
{
    ASSERT(pObject);

    _log=new CHistoryRevertable(name, pObject, acrionId, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, INT32 newVal, INT32 oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, INT64 newVal, INT64 oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, float newVal, float oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, double newVal, double oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, bool newVal, bool oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, Gdiplus::Point newVal, Gdiplus::Point oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, Gdiplus::Size newVal, Gdiplus::Size oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, Gdiplus::Rect newVal, Gdiplus::Rect oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, const CString& newVal, const CString & oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, Gdiplus::Image* newVal, Gdiplus::Image * oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::SetAttribute( CDrawObject * changed, UINT index, Gdiplus::ColorMatrix &newVal, Gdiplus::ColorMatrix& oldVal )
{
    _log=new CHistoryAttributeChanged(changed, index, newVal, oldVal, _log);
    _log=_log->optimize();
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, INT32 newVal, INT32 oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, INT64 newVal, INT64 oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, float newVal, float oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, double newVal, double oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, bool newVal, bool oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, Gdiplus::Point newVal, Gdiplus::Point oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, Gdiplus::Size newVal, Gdiplus::Size oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, Gdiplus::Rect newVal, Gdiplus::Rect oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, const CString& newVal, const CString & oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, Gdiplus::Image* newVal, Gdiplus::Image * oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}

void CHistroyLog::GroupSetAttribute( CDrawObject * changed, UINT index, Gdiplus::ColorMatrix &newVal, Gdiplus::ColorMatrix &oldVal )
{
    CHistory *pLog=new CHistoryAttributeChanged(changed, index, newVal, oldVal, NULL);
    _pGroup->addAction(pLog);
}
