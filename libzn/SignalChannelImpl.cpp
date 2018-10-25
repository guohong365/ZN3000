#include "stdafx.h"
#include "SignalChannelImpl.h"


SignalChannelImpl::SignalChannelImpl(const CString& label, double frequency, double physicalMinimum, double physicalMaximum, const CString& dimension, size_t bufferSize, const CString & sensor)
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

double SignalChannelImpl::getPhysicalMinimum() const
{
	return _physicalMinimum;
}

void SignalChannelImpl::setPhysicalMinimum(double physicalMinimum)
{
	_physicalMinimum=physicalMinimum;
}

double SignalChannelImpl::getPhysicalMaximum() const
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

double SignalChannelImpl::getHighPassFilter() const
{
	return _highPassFilter;
}

void SignalChannelImpl::setHighPassFilter(double highPass)
{
	_highPassFilter=highPass;
}

double SignalChannelImpl::getLowPassFilter() const
{
	return _lowPassFilter;
}

void SignalChannelImpl::setLowPassFilter(double lowPass)
{
	_lowPassFilter=lowPass;
}

double SignalChannelImpl::getNotchFilter() const
{
	return _notchFilter;
}

void SignalChannelImpl::setNotchFilter(double notch)
{
	_notchFilter=notch;
}

double SignalChannelImpl::getSampleFrequency() const
{
	return _sampleFrequency;
}

void SignalChannelImpl::setSampleFrequency(double sampleFrequency)
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

const SignalBuffer<double>& SignalChannelImpl::getSignalBuffer() const
{
	return _signalBuffer;
}

SignalBuffer<double>& SignalChannelImpl::getSignalBuffer()
{
	return _signalBuffer;
}

size_t SignalChannelImpl::getSampleCount() const
{
	return _signalBuffer.getLength();
}

void SignalChannelImpl::setPhysicalMaximum(double physicalMaximum)
{
	_physicalMaximum=physicalMaximum;
}
