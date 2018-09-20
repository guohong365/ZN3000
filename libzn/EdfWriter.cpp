#include "stdafx.h"
#include "EdfWriter.h"




EdfWriter::EdfWriter(const CString& fileName)
	:RecordWriter(fileName)
{
}

bool EdfWriter::write(const RheographyRecord* pRecord)
{
	return true;
}
