#pragma once
#include "RecordWriter.h"

class EdfWriter : RecordWriter
{
public:
	explicit EdfWriter(const CString & fileName);
	virtual bool write(const RheographyRecord* pRecord);
};

