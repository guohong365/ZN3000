#pragma once
#include "WaveDrawer.h"

class ActiveWaveDrawer : WaveDrawer
{
public:
	ActiveWaveDrawer(void);
	~ActiveWaveDrawer(void);
private:
	SIZE_T _current;
	double _beginTime;
};

