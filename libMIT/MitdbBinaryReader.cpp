#include "stdafx.h"
#include <uc/utils/utility.h>
#include <uc/signals/RecordReader.h>
#include "libwfdb/wfdblib.h"
#include "../libCore/SignalSeriesImpl.h"
#include "../libCore/RecordImpl.h"

#include "MitdbBinaryReader.h"
namespace uc
{
	namespace signals
	{
		MitdbBinaryReader::~MitdbBinaryReader()
		{
		}

        Record* MitdbBinaryReader::read(const TSTRING& recordName)
        {    
            std::string name;
#ifdef _UNICODE
            name= utils::wstombs(recordName);
#else
            name = recordName
#endif
            char *pName=const_cast<char*>(name.c_str());
            int ns=isigopen(pName, nullptr, 0);
            if(ns <= 0) return nullptr;
            WFDB_Siginfo *pSigInfo=new WFDB_Siginfo[ns];
            isigopen(pName, pSigInfo, ns);
            WFDB_Sample *v=new WFDB_Sample[ns];
            SignalSeriesImpl **ppLeadBuffer =new SignalSeriesImpl*[ns];
            Record * pRecord = new RecordImpl();
            for(int i = 0; i < ns; ++i)
            {
                ppLeadBuffer[i]=new SignalSeriesImpl(pSigInfo[0].nsamp, getifreq());
                //ppLeadBuffer[i]->setBaseline(pSigInfo[i].baseline/pSigInfo[i].gain);
                ppLeadBuffer[i]->scale(pSigInfo[i].gain);
                pRecord->add(ppLeadBuffer[i]);
            }
            for(int n=0; n < pSigInfo[0].nsamp; ++n)
            {
                getvec(v);
                for (int i= 0; i< ns ; ++i)
                {
                    double * p= ppLeadBuffer[i]->buffer();
                    p[n] = v[i]/pSigInfo[i].gain;
                }
            }
            delete[] pSigInfo;
            delete[] ppLeadBuffer;
            return pRecord;
        }

        Record* MitdbBinaryReader::read(FILE* fp)
        {
            return nullptr;
        }
	}
}
