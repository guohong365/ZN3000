#pragma once
#include <stack>
#include <algorithm>
#include "DrawObject.h"
#include "History.h"

class CHistoryManager;
class CHistroyLog
{
	CHistory * _log;
    CHistoryDataGroup * _pGroup;
	bool _inEditing;
    CHistoryManager *_pManager;
    bool _inGroupEditing;
public:
	CHistroyLog(CHistoryManager *pManager);
    bool IsInEditing();
	void BeginEdit();
	void AddObject(CDrawObject * changed, CDrawObject* added, CDrawObject * after);
	void DeleteObject(CDrawObject * changed, CDrawObject * deleted, CDrawObject * after);
	void SetAttribute(CDrawObject * changed, UINT index, INT32 newVal, INT32 oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, INT64 newVal, INT64 oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, float newVal, float oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, double newVal, double oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, bool newVal, bool oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, Gdiplus::Point newVal, Gdiplus::Point oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, Gdiplus::Size newVal, Gdiplus::Size oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, Gdiplus::Rect newVal, Gdiplus::Rect oldVal);

    void SetAttribute(CDrawObject * changed, UINT index, const CString& newVal, const CString & oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, Gdiplus::Image* newVal, Gdiplus::Image * oldVal);
    void SetAttribute(CDrawObject * changed, UINT index, Gdiplus::ColorMatrix &newVal, Gdiplus::ColorMatrix& oldVal);


	void SetOrder(CDrawObject * owner, CDrawObject * changed, CDrawObject * newAfter, CDrawObject * oldAfter);
    void SetRevertable(const CString& name, CDrawObject * pObject, int acrionId );
    void EndEdit();

    void BeginGroupEdit(const CString & name);
    void GroupAddObject(CDrawObject * changed, CDrawObject* added, CDrawObject * after);
    void GroupDeleteObject(CDrawObject * changed, CDrawObject * deleted, CDrawObject * after);

    void GroupSetAttribute(CDrawObject * changed, UINT index, INT32 newVal, INT32 oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, INT64 newVal, INT64 oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, float newVal, float oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, double newVal, double oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, bool newVal, bool oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, Gdiplus::Point newVal, Gdiplus::Point oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, Gdiplus::Size newVal, Gdiplus::Size oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, Gdiplus::Rect newVal, Gdiplus::Rect oldVal);

    void GroupSetAttribute(CDrawObject * changed, UINT index, const CString& newVal, const CString & oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, Gdiplus::Image* newVal, Gdiplus::Image * oldVal);
    void GroupSetAttribute(CDrawObject * changed, UINT index, Gdiplus::ColorMatrix &newVal, Gdiplus::ColorMatrix &oldVal);


    void GroupSetOrder(CDrawObject * owner, CDrawObject * changed, CDrawObject * newAfter, CDrawObject * oldAfter);
    void EndGroupEdit();

	CHistory * Detach();
	void Discard();
    CHistoryManager *manager();
};
class CHistoryManager  //implemented as cycled queue
{
	typedef	std::stack< CHistory *> UNDOSTACK;
	UNDOSTACK _undoStack;
	UNDOSTACK _redoStack;
    CHistroyLog *_pHistroyLog;
protected:
	CHistory * PopUndoData();
	CHistory * PopRedoData();
	void PushUndoData(CHistory *pUndoData);
	void PushRedoData(CHistory *pUndoData);

public:
    CHistoryManager();
	virtual ~CHistoryManager(void);
	void AddUndo(CHistory *pUndoData);
	void ClearRedoList();
	void ClearUndoList();
	void ClearAll();
	
	bool CanUndo();
	bool CanRedo();
	void Undo();
	void Redo();
	static CHistoryManager & undoManager();
	CHistroyLog & logger();
};

