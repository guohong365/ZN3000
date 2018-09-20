#include "stdafx.h"
#include "ObjectObserver.h"

//#include <DrawObject\ObjectsContainer.h>
using namespace Gdiplus;

void IObjectObserver::notifyChildAdded( CDrawObject * container, CDrawObject *childToAdd, CDrawObject *afterObject )
{
}

void IObjectObserver::notifyChildDeleted( CDrawObject *changedObject, CDrawObject *childToDelete, CDrawObject * afterObject )
{
}

void IObjectObserver::notifyAttribtesChanged( CDrawObject *changedObject, UINT attributeIndex,ATTR_VALUE & newVal, ATTR_VALUE & oldVal )
{

}

void IObjectObserver::notifyOrderChanged( CDrawObject* container, CDrawObject * changedObject, CDrawObject * newAfterObject, CDrawObject *oldAfterObject )
{
}

void IObjectObserver::notifyRevert( CDrawObject *changed, int &action )
{
    
}
