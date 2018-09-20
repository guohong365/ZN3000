#pragma once

class Sampler
{
public:
	virtual bool begin()=0;
	virtual void pause()=0;
	virtual void resume()=0;
	virtual void quit()=0;
	virtual ~Sampler(){}
};


