#pragma once
#include "dataloader.h"
class ScpLoader :public DataLoader
{
public:
	explicit ScpLoader(const CString&  name);
	void load(LPCTSTR lpFileName);
	~ScpLoader(void);
};

