#pragma once
#include "PatientInfo.h"

#define EDF_VERSION '0'
#define ZN_EXTENSION_IDENTIFY "ZN"
#define ZN_EXTENSION_VERSION "100"
#pragma pack(1)

struct EdfHeaderMap
{
	char Version[8];
	char PatientIdentification[80];
	char RecordIdentification[80];
	char StartDate[8];
	char StartTime[8];
	char Reserved[44];
	char RecordCount[8];
	char RecordDuration[8];
	char SignalCount[4];
	char SignalInfo[1];
};
#pragma pack()
struct EdfChannelParameterMap
{
	char *pLabel;
	char *pTransducerType;
	char *pPhysicalDimension;
	char *pPhysicalMinimum;
	char *pPhysicalMaximum;
	char *pDigitalMinimum;
	char *pDigitalMaximum;
	char *pFilters;
	char *pReserved;
};

class EdfHeader
{
public:
	CString ZnVersion;
	PatientInfo PatientInfo;

	int StartDateYear;
	int _startDateMonth;
	int _startDateDay;
	int _startTimeHour;
	int _startTimeMinute;
	int _startTimeSecond;
	CString _recordCode;
	CString _hospital;
	CString _department;
	int _recordCount;
	float _recordDuration;
	int _channelCount;

};