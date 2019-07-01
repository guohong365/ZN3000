#pragma once
#include <uc/libdrawing.h>
#include "WaveDrawer.h"

class ActiveWaveDrawer : uc::signals::visualization::WaveDrawer
{
public:
	ActiveWaveDrawer(void);
	~ActiveWaveDrawer(void);
private:
	SIZE_T _current;
	double _beginTime;
};

