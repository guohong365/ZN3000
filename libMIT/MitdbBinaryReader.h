#pragma once
#include <uc/libcore.h>
#include <uc/signals/RecordReader.h>
namespace uc
{
	namespace signals
	{
		class LIB_MITDB_CLASS MitdbBinaryReader : public RecordReader
		{
		public:
			virtual ~MitdbBinaryReader();
			virtual Record* read(const TSTRING & fileName);
			virtual Record* read(FILE* fp);
		};
	}
}

