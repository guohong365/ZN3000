#pragma once
#include <XTToolkitPro.h>
class IPropertyProvider
{

  public:
    virtual ~ IPropertyProvider(void)
    {
    };

    virtual void InitializePropertyGrid(CXTPPropertyGrid & propertyGrid) = 0;

    virtual void OnPropertyChanged(CXTPPropertyGridItem * pChangedItem) = 0;
};
