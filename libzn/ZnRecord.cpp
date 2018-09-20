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
                                                                                             _partId(partId),
                                                                                             _isAnalysisSection(
	                                                                                             isAnalysisSection)
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
}
