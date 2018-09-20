#pragma once

#include <map>
#include <vector>
#include "SignalBuffer.h"
#include "PatientInfo.h"
#define LEN_NAME 20
#define LEN_CODE 10
#define SEX_FEMALE 'F'
#define SEX_MALE   'M'

struct RecordInfo
{
	CString RecordId;
	CString Hospital;
	CString Department;
	int RecordCount;
	double RecordDuration;
	int ChannelCount;
};
struct AnnotationItem
{
	ULONG Offset;
	CString Label;
};

struct Annotations
{
	std::vector<AnnotationItem> Items;
	CString OwnerChannel;	
};
class ChannelInfo
{
public:
	CString _label;
	CString _sensor;
	CString _physicalDimension;
	double _physicalMinimum;
	double _physicalMaximum;
	int _digitalMinimum;
	int _digitalMaximum;
	double _highPassFilter;
	double _lowPassFilter;
	double _notchFilter;
	int _sampleCountPerRecord;
	int _totalSampleCount;
	int *_pData;
};
class RheographyRecord
{
public:
	RheographyRecord();
	virtual ~RheographyRecord(void);
private:	
	std::map<std::string, SignalBuffer<float>*> _buffers;
};

