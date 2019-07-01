#pragma once
#include <uc/libcore.h>
#include <uc/signals/RecordReader.h>
namespace uc
{
	namespace signals
	{		
		class RecordReader;
		class Record;
		class LIB_MITDB_CLASS MitdbTextReader : public RecordReader
		{
		public:
			virtual ~MitdbTextReader(void);
			virtual Record* read(const TSTRING & fileName);
			virtual Record* read(FILE* fp);
		};
	}
}

