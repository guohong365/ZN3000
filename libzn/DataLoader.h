#pragma once

#include <system_error>
#include "SignalBuffer.h"

class DataLoader
{
public:
	explicit DataLoader(const CString& name);
	const CString& getName() const;	
	virtual int load(LPCTSTR lpFileName, SignalBuffer<float>& waveBuffer);	
	virtual int onLoad(FILE * fp, SignalBuffer<float>& waveBuffer)=0;
	virtual ~DataLoader(void);
private:
	CString _name;
};

inline DataLoader::DataLoader(const CString& name)
	:_name(name)
{
}

inline const CString& DataLoader::getName() const
{
	return _name;
}

inline int DataLoader::load(LPCTSTR lpFileName, SignalBuffer<float>& waveBuffer)
{
	FILE * fp=nullptr;
	errno_t error=_tfopen_s(&fp,lpFileName, _T("rb"));
	if(fp==nullptr) return error;
	error = onLoad(fp, waveBuffer);
	fclose(fp);
	return error;
}

inline DataLoader::~DataLoader()
{
}

