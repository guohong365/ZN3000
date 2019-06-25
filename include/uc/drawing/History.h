#pragma  once
#include <algorithm>
#include <stack>
#include <vector>
class CHistory
{
protected:
	CString _strActionName;
	CDrawObject *_pChanged;
	CHistory *_pNext;
public:
	const CString & getName();
    CDrawObject * changed();
	CHistory * next();
	Gdiplus::Image * QueryImage();
	void undo();
	void redo();
    CHistory *optimize();
	virtual ~CHistory();

    virtual void releaseUndo();
    virtual void releaseRedo();

	static const CString getAttributeName(UINT index);

protected:
	CHistory(LPCTSTR lpszActionName, CDrawObject * pChanged, CHistory * pNext);
	virtual void onUndo(CDrawObject *responsor)=0;
	virtual void onRedo(CDrawObject *responsor)=0;
    virtual CHistory *onOptimaize();;
	virtual Gdiplus::Image * onQueryImage();
};

class release_history
{
public:
    void operator()(CHistory * pUndoData)
    {
        CHistory * pHistory=pUndoData;
        CHistory * pNext;
        while(pHistory)
        {
            pNext=pHistory->next();
            pHistory->releaseUndo();
            delete pHistory;
            pHistory=pNext;
        }
    }

};



class release_undo_history
{
public:
	void operator()(CHistory * pUndoData)
	{
		CHistory * pHistory=pUndoData;
		CHistory * pNext;
		while(pHistory)
		{
			pNext=pHistory->next();
            pHistory->releaseUndo();
			delete pHistory;
			pHistory=pNext;
		}
	}

};

class release_redo_history
{
public:
    void operator()(CHistory * pUndoData)
    {
        CHistory * pHistory=pUndoData;
        CHistory * pNext;
        while(pHistory)
        {
            pNext=pHistory->next();
            pHistory->releaseRedo();
            delete pHistory;
            pHistory=pNext;
        }
    }

};
class CHistoryAddObject:public CHistory
{
	CDrawObject *_pAdded;
	CDrawObject *_pAfter;
public:
	CHistoryAddObject(CDrawObject * pContainer, CDrawObject * pAdded, CDrawObject * pAfter, CHistory * pNext);
    virtual void releaseRedo();
private:
	virtual void onUndo(CDrawObject *responsor);
	virtual void onRedo(CDrawObject *responsor);
    virtual CHistory * onOptimaize();

};

class CHistoryDeleteObject:public CHistory
{
	CDrawObject * _pDeleted;
	CDrawObject * _pAfter;
public:
	CHistoryDeleteObject(CDrawObject * pContainer, CDrawObject * pDeleted, CDrawObject *pAfter, CHistory * pNext);
	virtual ~CHistoryDeleteObject();
    virtual void releaseUndo();
private:
	virtual void onUndo(CDrawObject *responsor);
	virtual void onRedo(CDrawObject *responsor); 
};


class CHistoryAttributeChanged:public CHistory
{
    ATTR_VALUE _newVal;
    ATTR_VALUE _oldVal;
	int _attributeIndex;
public:
	CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex,INT32 newVal,const INT32 oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex,INT64 newVal,INT64 oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex,float newVal, float oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex,double newVal,double oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex,bool newVal,bool oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex, Gdiplus::Size newVal, Gdiplus::Size oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex, Gdiplus::Point newVal,Gdiplus::Point oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex, Gdiplus::Rect newVal,Gdiplus::Rect oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex,Gdiplus::Image * newVal, Gdiplus::Image * oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex, const CString & newVal, const CString & oldVal, CHistory * pNext);
    CHistoryAttributeChanged(CDrawObject *pChanged, UINT attributeIndex, Gdiplus::ColorMatrix & newVal,  Gdiplus::ColorMatrix & oldVal, CHistory * pNext);

private:
	virtual void onUndo(CDrawObject *responsor);
	virtual void onRedo(CDrawObject *responsor);
    virtual void releaseRedo();
    virtual void releaseUndo();
    virtual CHistory * onOptimaize();
};

class CHistoryOrderChanged:public CHistory
{
	CDrawObject * _pChild;
	CDrawObject * _pNewAfter;
	CDrawObject * _pOldAfter;
public:
	CHistoryOrderChanged(CDrawObject * pContainer, CDrawObject* pChild, CDrawObject * pNewAfter, CDrawObject * pOldAfter, CHistory * pNext);
private:
	virtual void onUndo(CDrawObject *responsor);
	virtual void onRedo(CDrawObject *responsor);
};

//class CHistoryFontChanged:public CHistory
//{
//	FontInfo _newFont;
//	FontInfo _oldFont;
//public:
//	CHistoryFontChanged(CDrawObject *pChanged, const FontInfo & newFont, const FontInfo & oldFont, CHistory * pNext);
//private:
//	virtual void OnUndo(CDrawObject *responsor);
//	virtual void OnRedo(CDrawObject *responsor);
//};



class CHistoryDataGroup:public CHistory
{
protected:
    std::vector<CHistory *> _undoGroup;
    class group_undo_class
    {
    public:
        void operator()(  CHistory *  pUndoHistory)
        {
            pUndoHistory->undo();
        }
    };
    class group_redo_class
    {
    public:
        void operator ()( CHistory * pRedoHistory)
        {
            pRedoHistory->redo();
        }
    };
public:
	CHistoryDataGroup(const CString & name);
	virtual ~CHistoryDataGroup();
    virtual void onUndo(CDrawObject *responsor);
    virtual void onRedo(CDrawObject *responsor);
    virtual void releaseRedo();
    virtual void releaseUndo();
	void addAction(CHistory * pUndoData);
};

class CHistoryRevertable:public CHistory
{
    int _actionId;
public:
    CHistoryRevertable(const CString & name, CDrawObject *pChanged, int action, CHistory *pNext);
    virtual void onUndo(CDrawObject *responsor);
    virtual void onRedo(CDrawObject *responsor);
};