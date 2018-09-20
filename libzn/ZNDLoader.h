#pragma once


struct ZndFileBuffer;

class ZNDLoader
{
	static int setupPointer(ZndFileBuffer * pzndData);
public:
	static int load(const TCHAR * fileName, ZndFileBuffer * pzndData);
	static int load(CFile * file, ZndFileBuffer * pzndData);
};
