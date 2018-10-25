#pragma once
#include <vector>
#include <libUC.h>
#include <string>
#include <array>

typedef struct DataHdr {
        char hdr[4];
        unsigned int size;
        float sr;
        unsigned char bits;
        unsigned char lead;
        unsigned short umV;
        unsigned short baseLine;
        unsigned char hh;
        unsigned char mm;
        unsigned char ss;
        char reserved[19];
} DATA_HDR, *PDATA_HDR;

class MitbihAnnotation
{
    
};

template class LIB_UC_API std::allocator<char>;
template class LIB_UC_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;


class LIB_UC_API LeadBuffer
{
public:
    explicit LeadBuffer(size_t count=0, const double* pBuffer=nullptr);

    ~LeadBuffer();

    const std::string& getName() const;

    void setName(const char* name);
    const std::string& getUnit() const;

    void setUnit(const char* unit);

    double getGain() const;
    void setGain(double gain);

    double getBaseline() const;

    void setBaseline(double baseline);
    double getSampleRate() const;

    void setSampleRate(double sampleRate);

    double* getData();
    const double* getData() const;
    void setData(const double* pData, size_t count);

    size_t getCount() const;
    void setCount(size_t count);

    void normalize();
    bool isNormalized() const;
private:
    std::string _name;
    std::string _unit;
    double _sampleRate;
    double _gain;
    double _baseline;
    double _max;
    double _min;
    double _average;
    bool _normalized;
    size_t _count;
    double * _pBuffer;
};

 template class LIB_UC_API std::allocator<LeadBuffer*>;
 template class LIB_UC_API std::vector<LeadBuffer*>;
 template class LIB_UC_API std::_Vector_val<LeadBuffer*, std::allocator<LeadBuffer*>>;

class LIB_UC_API MitbihRecord
{
public:
    MitbihRecord();
    ~MitbihRecord();
    int getCount();;
    void addLead(LeadBuffer* pLead);
    LeadBuffer* get(int index);
private:
    std::vector<LeadBuffer *> _leads;

};
class LIB_UC_API MitbihReader
{
public:
    virtual ~MitbihReader(){}
    virtual MitbihRecord* load(const TCHAR * recordName)=0;
    
};
class LIB_UC_API MitbihBinaryReader : public MitbihReader
{
public:
    static void init(const TCHAR * databasePath);
    static void init(char * databasePath);
    virtual MitbihRecord* load(const TCHAR * recordName);
    static MitbihRecord* load(char * recordName);

};

class LIB_UC_API MitbihTextReader : public MitbihReader
{
public:
    virtual MitbihRecord* load(const TCHAR * recordName);
};




