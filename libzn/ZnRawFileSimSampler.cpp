#include "stdafx.h"
#include "ZnRowFileSimSampler.h"
#include <stdexcept>
#include <uc/signals/SerialPortSampler.h>

#define PACKET_SIZE sizeof(Packet)
#define TEST_BUFFER_SIZE (PACKET_SIZE*3)

class FileLength
{
public:
    SIZE_T operator()(const CString& fileName) const
    {
        _stat64 stat;
        if(_tstat64(fileName, &stat)==0)
        {
            return SIZE_T(stat.st_size);
        }
        return 0;        
    }
    SIZE_T operator()(FILE*fp) const
    {
        const __int64 current=_ftelli64(fp);
        _fseeki64(fp ,0, SEEK_END);
        const __int64 length=_ftelli64(fp);
        _fseeki64(fp ,current, SEEK_SET);
        return SIZE_T(length);
    }
};

int ZnRowFileSimSampler::load(FILE* fp, SignalBuffer<float> * &pBuffer)
{
    const SIZE_T length=FileLength()(fp);

	pBuffer=new SignalBuffer<float>(length);

	if(fread(pBuffer->getBuffer(), length, 1, fp)!= 1)
	{
		return -1;
	}	
	return 0;
}
ZnRowFileSimSampler::ZnRowFileSimSampler(const CString& fileDir)
    : AbstractSampler(4)
      , _fileDir(fileDir)
      , _current(0)
{
    _pData[0] = _pData[1] = _pData[2] = _pData[3] = nullptr;
    CString fileName;
    for (int i = 0; i < 4; ++i)
    {
        fileName.Format(_T("data.%d.dat"), i + 1);
        FILE* fp;
        _tfopen_s(&fp, fileName, _T("rb"));
        if (fp)
        {
            continue;
        }
        load(fp, _pData[i]);
    }
}


ZnRowFileSimSampler::~ZnRowFileSimSampler()
{
    for(SIZE_T i=0; i< AbstractSampler::getCount(); ++i )
    {
        delete _pData[i];
    }
}

bool ZnRowFileSimSampler::onBeforeStart()
{
    resetAll();
    return true;
}

void ZnRowFileSimSampler::doSampler()
{
    for(SIZE_T i=0; i< getCount(); ++i)
    {
        SignalBuffer<double> *pBuffer=get(i);
        if(pBuffer && _pData[i])
        {                
            double val=(*_pData[i])[_current];
            pBuffer->append(val);
        }
    }
    _current ++;
    Sleep(1);
}

void ZnRowFileSimSampler::resetAll()
{
    for(SIZE_T i=0; i< getCount(); ++i)
    {
        SignalBuffer<double> *pBuffer=get(i);
        if(pBuffer)
        {                
            pBuffer->setLength(0);
        }
        _current=0;
    }
}
