#include "stdafx.h"
#include "MitbihRecord.h"
#include "libwfdb/wfdblib.h"
#include <tchar.h>
#include <stdlib.h>
LeadBuffer::LeadBuffer(size_t count, const double* pBuffer)
    : _sampleRate(0)
    , _gain(1)
    , _baseline(0)
    , _max(0)
    , _min(0)
    , _average(0)
    , _normalized(false),
                                                             _count(count)
{
    if (_count == 0)
    {
        _pBuffer = nullptr;
        return;
    }
    _pBuffer = new double[_count];
    if (pBuffer)
    {
        memcpy(_pBuffer, pBuffer, sizeof(double) * _count);
    }
    else
    {
        memset(_pBuffer, 0, sizeof(double) * _count);
    }
}

LeadBuffer::~LeadBuffer()
{
    delete [] _pBuffer;
}

const std::string& LeadBuffer::getName() const
{
    return _name;
}

void LeadBuffer::setName(const char* name)
{
    _name = name;
}

const std::string& LeadBuffer::getUnit() const
{
    return _unit;
}

void LeadBuffer::setUnit(const char* unit)
{
    _unit = unit;
}

double LeadBuffer::getGain() const
{
    return _gain;
}

void LeadBuffer::setGain(double gain)
{
    _gain = gain;
}

double LeadBuffer::getBaseline() const
{
    return _baseline;
}

void LeadBuffer::setBaseline(double baseline)
{
    _baseline = baseline;
}

double LeadBuffer::getSampleRate() const
{
    return _sampleRate;
}

void LeadBuffer::setSampleRate(double sampleRate)
{
    _sampleRate = sampleRate;
}

double* LeadBuffer::getData()
{
    return _pBuffer;
}

const double* LeadBuffer::getData() const
{
    return _pBuffer;
}

void LeadBuffer::setData(const double* pData, size_t count)
{
    delete []_pBuffer;
    _count = count;
    _pBuffer = new double[_count];
    memcpy(_pBuffer, pData, sizeof(double)*_count);
}

size_t LeadBuffer::getCount() const
{
    return _count;
}

void LeadBuffer::setCount(size_t count)
{
    delete _pBuffer;
    _count = count;
    if (_count > 0)
    {
        _pBuffer = new double[_count];
        memset(_pBuffer, 0, sizeof(double) * _count);
    }
}

void LeadBuffer::normalize()
{
    if(_count == 0) return;
    double sum = _pBuffer[0];
    _min = _max = _pBuffer[1];
    for(size_t i=0; i< _count; ++i)
    {
        if(_min > _pBuffer[i]) _min = _pBuffer[i];
        if(_max < _pBuffer[i]) _max = _pBuffer[i];
        sum += _pBuffer[i];        
    }
    _average = sum / _count;
    _gain = _max - _min;
    if(_gain==0) return;
    for(size_t i=0; i< _count; ++i)
    {
        _pBuffer[i] = _pBuffer[i]/_gain;
    }

}

bool LeadBuffer::isNormalized() const
{
    return _normalized;
}

MitbihRecord::MitbihRecord()
{
}


MitbihRecord::~MitbihRecord()
{
    for(std::vector<LeadBuffer*>::iterator it= _leads.begin(); it!=_leads.end(); ++it)
    {
        delete *it;
    }
}

int MitbihRecord::getCount()
{
    return _leads.size();
}

void MitbihRecord::addLead(LeadBuffer* pLead)
{
    _leads.push_back(pLead);
}

LeadBuffer* MitbihRecord::get(const int index)
{
    return _leads.at(index);
}

void MitbihBinaryReader::init(const TCHAR* databasePath)
{
    char path[256];
#ifdef _UNICODE
    wcstombs_s(nullptr, path, 256, databasePath, 256);
#else
    strncpy(path, databasePath, 255);
    path[255]=0;
#endif
    setwfdb(path);
}

void MitbihBinaryReader::init(char* databasePath)
{
    setwfdb(databasePath);
}

MitbihRecord* MitbihBinaryReader::load(const TCHAR* recordName)
{
    char record[256]="";
#ifdef _UNICODE
    wcstombs_s(nullptr,record, 256, recordName, 256);
#else
    strncpy(record, recordName,255 );
    record[255]=0;
#endif
    return load(record);
}

MitbihRecord* MitbihBinaryReader::load(char* recordName)
{
    int ns=isigopen(recordName, nullptr, 0);
    if(ns <= 0) return nullptr;
    WFDB_Siginfo *pSigInfo=new WFDB_Siginfo[ns];
    isigopen(recordName, pSigInfo, ns);
    WFDB_Sample *v=new WFDB_Sample[ns];
    LeadBuffer **ppLeadBuffer =new LeadBuffer*[ns];
    MitbihRecord * pRecord = new MitbihRecord();
    for(int i = 0; i < ns; ++i)
    {
        ppLeadBuffer[i]=new LeadBuffer(pSigInfo[0].nsamp);
        ppLeadBuffer[i]->setBaseline(pSigInfo[i].baseline/pSigInfo[i].gain);
        ppLeadBuffer[i]->setGain(pSigInfo[i].gain);
        ppLeadBuffer[i]->setSampleRate(getifreq());
        pRecord->addLead(ppLeadBuffer[i]);
    }
    for(int n=0; n < pSigInfo[0].nsamp; ++n)
    {
        getvec(v);
        for (int i= 0; i< ns ; ++i)
        {
            double * p= ppLeadBuffer[i]->getData();
            p[n] = v[i]/pSigInfo[i].gain;
        }
    }
    delete[] pSigInfo;
    delete[] ppLeadBuffer;
    return pRecord;
}

MitbihRecord* MitbihTextReader::load(const TCHAR* recordName)
{
    FILE *fp;
    _tfopen_s(&fp, recordName, _T("rt"));
    if(!fp) return nullptr;
    
    
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
    while(!feof(fp))
    {
        fgets(buffer,BUFSIZ, fp);
        token = strtok_s(buffer, delim, &context);
        double time= atof(token);
        columns[0].push_back(time);
        for(int i = 1; i< count; i++)
        {
            token = strtok_s(nullptr, delim, &context);
            double val=atof(token);
            columns[i].push_back(val);
        }
    }
    fclose(fp);
    MitbihRecord * pRecord=new MitbihRecord();
    const double sampleRate= 1/(columns[0][1] - columns[0][0]);

    for(int i= 1; i< count; ++i)
    {
        LeadBuffer *pLead=new LeadBuffer(columns[i].size());
        pLead->setSampleRate(sampleRate);        
        for(int j=0; j< pLead->getCount(); ++j)
        {
            pLead->getData()[j]=columns[i][j];
        }
        pLead->normalize();
        pRecord->addLead(pLead);
    }
    delete[] columns;
    return pRecord;

}
