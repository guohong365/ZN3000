#pragma once
#include "Record.h"

class RecordReader
{
	FILE* _pFile;
public:
	explicit RecordReader(const CString & fileName);
	virtual ~RecordReader();
	virtual RheographyRecord * read()=0;
};


