#include "stdafx.h"
#include "RecordReader.h"

RecordReader::RecordReader(const CString& fileName)
	: _pFile(nullptr)
{
	_tfopen_s(&_pFile, fileName, _T("rb"));
}

RecordReader::~RecordReader()
{
	if (_pFile)
	{
		fclose(_pFile);
	}
}
