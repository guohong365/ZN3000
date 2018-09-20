#pragma once
#include "Record.h"

class RecordWriter
{
	FILE *_pFile;
public:
	explicit RecordWriter(const CString & fileName);
	virtual ~RecordWriter(void);
	virtual bool write(const RheographyRecord *pRecord)=0;
};

