#pragma once
#include "ZNDData.h"
#include <uc/rheography/RheographyRecordImpl.h>
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
	virtual SignalChannel * getChannel(const CString& label)=0;
	virtual SignalChannel * getAnalysisSection(PartId partId)=0;
	virtual void addChannel(SignalChannel * pChannel)=0;
};

class ZnRecordImpl : public RheographyRecordImpl, public ZnRecord
{
public:
	virtual bool isExamined(PartId partId);
	virtual SignalChannel* getChannel(PartId partId);
	virtual SignalChannel * getChannel(const CString& label);
	virtual SignalChannel* getAnalysisSection(PartId partId);
	virtual void addChannel(SignalChannel * pChannel);
};

class ZnHelper
{

public:
	static CString getPartLabel(PartId partId, bool isAnalysisSection)
	{
		return isAnalysisSection ? BODY_STRING[partId] : BODY_STRING[partId] + CString(" A");
	}
	static SignalChannel* createSignalChannel(PartId partId, double frequency, SIZE_T bufferSize)
	{
		return new ZnSignalChannelImpl(frequency, 0, 43.45, _T("mƱ"), bufferSize,
		                               partId == PART_HEART ? _T("Differential") : _T("Admittance"), partId);
	}
	static SignalChannel* createAnalysisSection(PartId partId, SIZE_T offset, SIZE_T length);
};



