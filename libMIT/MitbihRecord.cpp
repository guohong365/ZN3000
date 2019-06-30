#include "stdafx.h"
#include <uc/utils/utility.h>
#include "MitbihRecord.h"
#include "libwfdb/wfdblib.h"
#include <tchar.h>
#include <stdlib.h>
namespace uc
{
    namespace signals
    {
        void wfdbSetup(const char* database_path)
        {
            char * path=const_cast<char*>(database_path);
            setwfdb(path);
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
            SignalImpl **ppLeadBuffer =new SignalImpl*[ns];
            Record * pRecord = new RecordImpl();
            for(int i = 0; i < ns; ++i)
            {
                ppLeadBuffer[i]=new SignalImpl(pSigInfo[0].nsamp, getifreq());
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
            const char* delim=",";
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
                SignalImpl *pLead=new SignalImpl(columns[i].size(), sampleRate);
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
