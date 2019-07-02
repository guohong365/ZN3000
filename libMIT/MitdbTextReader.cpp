#include "stdafx.h"
#include <uc/utils/utility.h>
#include <uc/signals/RecordReader.h>
#include "libwfdb/wfdblib.h"
#include "../libCore/SignalSeriesImpl.h"
#include "../libCore/RecordImpl.h"
#include "MitdbTextReader.h"
namespace uc
{
	namespace signals
	{
		MitdbTextReader::~MitdbTextReader()
		{
		}

        Record* MitdbTextReader::read(const TSTRING& recordName)
        {
            FILE *fp;
            _tfopen_s(&fp, recordName.c_str(), _T("rt"));
            if(!fp) return nullptr;
            Record * pRecord=read(fp);
            fclose(fp);
            return pRecord;
        }
        Record* MitdbTextReader::read(FILE* fp)
        {
            char buffer[256];
            fgets(buffer,BUFSIZ, fp);
            char *context;
            const char* delim="\t ,";
            char* token = strtok_s(buffer, delim, &context);
            int count =0;
            while(token)
            {
                token=strtok_s(nullptr, delim, &context);
                if(token) count ++;
            }
            std::vector<double>* columns=new std::vector<double>[count];
            long index =0;
            while(!feof(fp) && fgets(buffer,BUFSIZ, fp)){
                token = strtok_s(buffer, delim, &context);
                double time= atof(token);
                columns[0].push_back(time);
                for(int i = 1; i< count; i++)
                {
                    token = strtok_s(nullptr, delim, &context);
                    double val=atof(token);
                    columns[i].push_back(val);
                }
                index ++;
            }
            fclose(fp);
            Record * pRecord=new RecordImpl();
            const double sampleRate=int(1/(columns[0][1] - columns[0][0]) + 0.5);

            for(int i= 1; i< count; ++i)
            {
                SignalSeriesImpl *pLead=new SignalSeriesImpl(columns[i].size(), sampleRate);
                for(int j=0; j< pLead->sampleCount(); ++j)
                {
                    pLead->buffer()[j]=columns[i][j];
                }
                pLead->normalize();
                pRecord->add(pLead);
            }
            delete[] columns;
            return pRecord;

        }
	}
}