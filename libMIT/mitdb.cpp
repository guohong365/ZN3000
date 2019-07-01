#include "stdafx.h"
#include <uc/utils/utility.h>
#include "libwfdb/wfdblib.h"
#include <tchar.h>
#include <stdlib.h>
#include "../libCore/RecordImpl.h"
#include "../libCore/SignalSeriesImpl.h"

namespace uc
{
    namespace signals
    {
		void initMitdb()
		{
			
		}
        void wfdbSetup(const char* database_path)
        {
            char * path=const_cast<char*>(database_path);
            setwfdb(path);
        }
    }
}
