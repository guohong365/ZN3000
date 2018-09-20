#pragma once
#include "ZNDData.h"
#include "RheographyRecordImpl.h"
#include "SignalChannelImpl.h"


class ZnSignalChannel
{
public:
	virtual ~ZnSignalChannel(){}
	virtual PartId getPartId() const=0;
	virtual void setPartId(PartId partId)=0;
	virtual bool isAnalysisSection(PartId partId)const=0;
};

class ZnAnalysisSection
{
public:
	virtual ~ZnAnalysisSection(){}
	virtual SIZE_T getOffset() const=0;
	virtual void setOffset(SIZE_T offset)=0;
};

class ZnSignalChannelImpl : public SignalChannelImpl, public ZnSignalChannel
{
public:
	ZnSignalChannelImpl(double frequency, double physicalMinimum,
	                    double physicalMaximum, const CString& dimension, SIZE_T bufferSize,
	                    const CString& sensor,
	                    PartId partId, bool isAnalysisSection = false);
	virtual PartId getPartId() const ;
	virtual void setPartId(PartId partId);
	virtual bool isAnalysisSection(PartId partId) const;
private:
	PartId _partId;
};

class ZnRecord
{
public:
	virtual ~ZnRecord(){}
	virtual bool isExamined(PartId partId)=0;
	virtual SignalChannel * getChannel(PartId partId)=0;
	virtual SignalChannel * getAnalysisSection(PartId partId)=0;
};

class ZnRecordImpl : public RheographyRecordImpl, public ZnRecord
{
public:
	virtual bool isExamined(PartId partId);
	virtual SignalChannel* getChannel(PartId partId);
	virtual SignalChannel* getAnalysisSection(PartId partId);
private:

};

class ZnHelper
{
	const static TCHAR partLabel[][20]={
		_T("HEART"),
		_T("LUNG"),
		_T("LIVER"),
		_T("KIDNEY"),
		_T("LEFT_INTERNAL_CAROTID_ARTERY"),
		_T("RIGHT_INTERNAL_CAROTID_ARTERY"),
		_T("LEFT_VERTEBRAL_ARTERY"),
		_T("RIGHT_VERTEBRAL_ARTERY"),
		_T("ARM"),
		_T("LEFT_LEG"),
		_T("RIGHT_LEG"),
		_T("HEART_ECG")
	};
public:
	static CString getPartLabel(PartId partId, bool isAnalysisSection)
	{
		return isAnalysisSection ? BODY_STRING[partId] : BODY_STRING[partId] + CString(" A");
	}
	static ZnSignalChannel* createSignalChannel(PartId partId, double frequency)
	{
		return new ZnSignalChannelImpl(frequency, 0, 43.45, _T("mƱ"), ZND_DATA_SIZE,
		                               partId == HEART ? _T("Differential") : _T("Admittance"), partId);
	}
	static ZnSignalChannel* createAnalysisSection(PartId partId, SIZE_T offset, SIZE_T length);
};



