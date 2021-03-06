#include "StdAfx.h"
#include "SymbolManager.h"
#include "CharSymbol.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

const WCHAR * symbolChar=_T("▲△○●→←↑↓▼▽■□★☆⇧⇩①②③④⑤⑥⑦⑧⑩⑪⑫⑬⑭⑮⑯⑰⑱⑲⑳ⒶⓏⓐⓩ⓪");

void CSymbolManager::Add( CDrawObject *pSymbol )
{
	CSymbolObject *pSymbolObject=DYNAMIC_DOWNCAST(CSymbolObject, pSymbol);
	if(pSymbolObject)
	{
		Add(pSymbolObject->GetCategory(), pSymbol);
	}
}

void CSymbolManager::Add( const CString& category, CDrawObject *pItem )
{
	ASSERT(pItem->IsKindOf(RUNTIME_CLASS(CSymbolObject)));

	void *pVoid;
	CDrawObjectList *pList;
	if(_Symbols.Lookup(category, pVoid))
	{
		pList=(CDrawObjectList*)pVoid;
	}
	else
	{
		pList=new CDrawObjectList();
		_Symbols.SetAt(category, pList);
	}
	ASSERT(pList);
	pList->AddTail(pItem);			
}

bool CSymbolManager::SaveSymbols( const CString &filename )
{
	CFile file;
	if(file.Open(filename, CFile::modeWrite|CFile::modeCreate))
	{
		CArchive ar(&file, CArchive::store);
		TRY 
		{
			CString key;
			void *pValue;
			CDrawObjectList *pList;
			ar << _Symbols.GetCount();
			POSITION pos=_Symbols.GetStartPosition();
			while(pos)
			{
				_Symbols.GetNextAssoc(pos, key, pValue);
				pList=(CDrawObjectList*)pValue;
				ar << key;
				ar << pList;
			}
		}
		CATCH (CException, e)
		{
			ar.Close();
			return false;
		}
		END_CATCH
		ar.Close();
		return true;
	}
	return false;
}

bool CSymbolManager::LoadSymbols( const CString &filename )
{
	CFile file;
	if(file.Open(filename, CFile::modeRead))
	{
		CArchive ar(&file, CArchive::load);
		TRY
		{
			CDrawObjectList *pList;
			CString key;
			int count;
			ar >> count;
			for(int i=0; i< count; i++)
			{
				ar >> key;
				ar >> pList;
				_Symbols.SetAt(key, pList);
			}	
		}
		CATCH(CException, e)
		{
			TRACE(_T("Load symbol failed!"));
			ar.Close();
			return false;
		}
		END_CATCH
		ar.Close();
		return true;
	}
	return false;
}

CSymbolManager::CSymbolManager()
{

}

CSymbolManager::~CSymbolManager()
{
	POSITION posMap=_Symbols.GetStartPosition();
	while(posMap)
	{
		CString key;
		void *pValue;
		CDrawObjectList *pList;
		_Symbols.GetNextAssoc(posMap, key, pValue);
		pList=(CDrawObjectList*)pValue;

		POSITION posObject=pList->GetHeadPosition();
		while(posObject)
		{
			CDrawObject *pObject=(CDrawObject*)pList->GetNext(posObject);
			delete pObject;
		}
		delete pList;
	}
}

int CSymbolManager::GetImageList( const CString & category, CImageList &imageList, int w, int h, Color background )
{
	CDrawObjectList *pList=NULL;
	void *pValue;
	if(_Symbols.Lookup(category, pValue))
	{
		ASSERT(pValue);
		pList=(CDrawObjectList*)pValue;
		HICON hIcon;
		CSymbolObject *pSymbol;
		POSITION pos=pList->GetHeadPosition();
		while(pos)
		{
			pSymbol=(CSymbolObject*)pList->GetNext(pos);
			//hBitmap=pSymbol->GetSymbolImage(w,h, background);
   //         imageList.Add(CBitmap::FromHandle(hBitmap), (CBitmap*)NULL);

            hIcon=pSymbol->GetSymbolIcon(w,h, background);
            imageList.Add(hIcon);
		}
		return pList->GetCount();
	}
	return 0;
}

int CSymbolManager::GetSymbolCategoryCount()
{
	return _Symbols.GetCount();
}

CString CSymbolManager::GetCategoryName( int index )
{
	CString key;
	void *pValue;
	int count=0;
	POSITION pos=_Symbols.GetStartPosition();
	while(pos)
	{
		_Symbols.GetNextAssoc(pos, key, pValue);
		if(index==count)
		{
			return key;
		}
		count ++;
	}
	return _T("");
}

CDrawObjectList * CSymbolManager::GetSymbolList( LPCTSTR lpszCategory )
{
	void *pValue;
	if(_Symbols.Lookup(lpszCategory, pValue))
	{
		return (CDrawObjectList *)pValue;
	}
	return NULL;
}

CMapStringToPtr & CSymbolManager::GetSymbolMap()
{
	return _Symbols;
}

void CSymbolManager::Remove( CDrawObject * pSymbol )
{
    ASSERT(pSymbol);
    void *pVoid;
    CDrawObjectList *pList;
    CSymbolObject * pItem=DYNAMIC_DOWNCAST(CSymbolObject, pSymbol);
    ASSERT(pItem);
    if(_Symbols.Lookup(pItem->GetCategory(), pVoid))
    {
        pList=(CDrawObjectList*)pVoid;
        if(pList)
        {
            POSITION pos=pList->Find(pItem);
            if(pos)
            {
                pList->RemoveAt(pos);
                delete pItem;
            }
        }
    }
}
