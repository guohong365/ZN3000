#include "stdafx.h"
#include "RecordWriter.h"


RecordWriter::RecordWriter(const CString & fileName)
	: _pFile(nullptr)
{
	_tfopen_s(&_pFile, fileName, _T("wr"));
}


RecordWriter::~RecordWriter(void)
{
	if(_pFile)
	{
		fclose(_pFile);
	}
}
