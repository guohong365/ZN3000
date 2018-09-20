#include "stdafx.h"
#include "ZnRecord.h"

ZnSignalChannelImpl::ZnSignalChannelImpl(const double frequency, const double physicalMinimum,
                                         const double physicalMaximum, const CString& dimension,
                                         const SIZE_T bufferSize, const CString& sensor,
                                         const PartId partId, const bool isAnalysisSection): SignalChannelImpl(
	                                                                                             _T(""), frequency,
	                                                                                             physicalMinimum,
	                                                                                             physicalMaximum,
	                                                                                             dimension, bufferSize,
	                                                                                             sensor),
                                                                                             _partId(partId)
{
	SignalChannelImpl::setLabel(BODY_STRING[partId]);
}

PartId ZnSignalChannelImpl::getPartId() const
{
	return _partId;
}

void ZnSignalChannelImpl::setPartId(PartId partId)
{
	_partId=partId;
}

bool ZnSignalChannelImpl::isAnalysisSection(PartId partId) const
{
	return getLabel().Compare(ZnHelper::getPartLabel(partId, true)) ==0;
}

bool ZnRecordImpl::isExamined(const PartId partId)
{
	return getChannel(partId) != nullptr;
}

SignalChannel* ZnRecordImpl::getChannel(PartId partId)
{
	for(SignalChannels::iterator i = getSignalChannels().begin(); i!= getSignalChannels().end(); ++i)
	{
		if(dynamic_cast<ZnSignalChannel*>(*i)->getPartId()==partId)
		{
			return *i;
		}
	}
	return nullptr;
}

SignalChannel* ZnRecordImpl::getChannel(const CString& label)
{
	for(SignalChannels::iterator i = getSignalChannels().begin(); i!= getSignalChannels().end(); ++i)
	{
		if((*i)->getLabel().Compare(label)==0) return *i;
		
	}
	return nullptr;
}

SignalChannel* ZnRecordImpl::getAnalysisSection(PartId partId)
{
	return getChannel(ZnHelper::getPartLabel(partId, true));
}

void ZnRecordImpl::addChannel(SignalChannel* pChannel)
{
	getSignalChannels().push_back(pChannel);
}
