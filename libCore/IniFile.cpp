#include "stdafx.h"
#include <uc/utils/IniFile.h>
#include "IniFileImpl.h"

namespace uc
{
	namespace utils
	{
		IniFile::~IniFile()
		{
		}

		IniFile* IniFile::OpenIniFile(const _TCHAR* IniFileName)
		{
			IniFileImpl* pIniFile = new IniFileImpl();
			pIniFile->OpenIniFile(IniFileName);
			return pIniFile;
		}
	}
}
