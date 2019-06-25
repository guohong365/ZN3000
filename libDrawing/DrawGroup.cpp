#include "StdAfx.h"
#include "drawgroup.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CDrawGroup, CDrawObject, 0)

CDrawGroup::CDrawGroup(void)
{
}

CDrawGroup::~CDrawGroup(void)
{
}

CDrawGroup* CDrawGroup::CreateGroup( CDrawObjectList * pObjects, CDrawObject * pParent/*=NULL*/ )
{
	CDrawGroup *pGroup;
	CDrawObject *pObject;
	if(pObjects==NULL || pObjects->IsEmpty())
		return NULL;
	pGroup=DYNAMIC_DOWNCAST(CDrawGroup, RUNTIME_CLASS(CDrawGroup)->CreateObject());
	ASSERT(pGroup);
    POSITION pos=pObjects->GetHeadPosition();
	while(pos)
	{
		pObject=pObjects->GetNext(pos);
		pObject->SetParent(pGroup);
        pGroup->AddToGroup(pObject);        		
	}
	pGroup->SetParent(pParent);
	return pGroup;
}