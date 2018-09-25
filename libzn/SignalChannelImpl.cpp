#include "stdafx.h"
#include "SignalChannelImpl.h"


SignalChannelImpl::SignalChannelImpl(const CString& label, double frequency, double physicalMinimum, double physicalMaximum, const CString& dimension, SIZE_T bufferSize, const CString & sensor)
	: _label(label)
	, _sensor(sensor)
	, _physicalDimension(dimension)
	, _physicalMinimum(physicalMinimum)
	, _physicalMaximum(physicalMaximum)
	, _digitalMinimum(-32768)
	, _digitalMaximum(32767)
	, _highPassFilter(-1)
	, _lowPassFilter(-1)
	, _notchFilter(-1)
	, _sampleFrequency(frequency)
	, _signalBuffer(bufferSize)
{
}


SignalChannelImpl::~SignalChannelImpl(){}

const CString& SignalChannelImpl::getLabel() const
{
	return _label;
}

void SignalChannelImpl::setLabel(const CString& label)
{
	_label=label;
}

const CString& SignalChannelImpl::getSensor() const
{
	return _sensor;
}

void SignalChannelImpl::setSensor(const CString& sensor)
{
	_sensor=sensor;
}

const CString& SignalChannelImpl::getPhysicalDimension()
{
	return _physicalDimension;
}

void SignalChannelImpl::setPhysicalDimension(const CString& dimension)
{
	_physicalDimension=dimension;
}

DOUBLE SignalChannelImpl::getPhysicalMinimum() const
{
	return _physicalMinimum;
}

void SignalChannelImpl::setPhysicalMinimum(DOUBLE physicalMinimum)
{
	_physicalMinimum=physicalMinimum;
}

DOUBLE SignalChannelImpl::getPhysicalMaximum() const
{
	return _physicalMaximum;
}

long SignalChannelImpl::getDigitalMinimum() const
{
	return _digitalMinimum;
}

void SignalChannelImpl::setDigitalMinimum(long digitalMinimum)
{
	_digitalMinimum=digitalMinimum;
}

long SignalChannelImpl::getDigitalMaximum() const
{
	return _digitalMaximum;
}

void SignalChannelImpl::setDigitalMaximum(long digitalMaximum)
{
	_digitalMaximum=digitalMaximum;
}

DOUBLE SignalChannelImpl::getHighPassFilter() const
{
	return _highPassFilter;
}

void SignalChannelImpl::setHighPassFilter(DOUBLE highPass)
{
	_highPassFilter=highPass;
}

DOUBLE SignalChannelImpl::getLowPassFilter() const
{
	return _lowPassFilter;
}

void SignalChannelImpl::setLowPassFilter(DOUBLE lowPass)
{
	_lowPassFilter=lowPass;
}

DOUBLE SignalChannelImpl::getNotchFilter() const
{
	return _notchFilter;
}

void SignalChannelImpl::setNotchFilter(DOUBLE notch)
{
	_notchFilter=notch;
}

DOUBLE SignalChannelImpl::getSampleFrequency() const
{
	return _sampleFrequency;
}

void SignalChannelImpl::setSampleFrequency(DOUBLE sampleFrequency)
{
	_sampleFrequency=sampleFrequency;
}

const Annotations& SignalChannelImpl::getAnnotations() const
{
	return _annotations;
}

Annotations& SignalChannelImpl::getAnnotations()
{
	return _annotations;
}

const SignalBuffer<float>& SignalChannelImpl::getSignalBuffer() const
{
	return _signalBuffer;
}

SignalBuffer<float>& SignalChannelImpl::getSignalBuffer()
{
	return _signalBuffer;
}

SIZE_T SignalChannelImpl::getSampleCount() const
{
	return _signalBuffer.getLength();
}

void SignalChannelImpl::setPhysicalMaximum(double physicalMaximum)
{
	_physicalMaximum=physicalMaximum;
}
