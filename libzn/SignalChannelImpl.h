#pragma once
#include <uc/signals/Record.h>

class SignalChannelImpl : public SignalChannel
{
public:
	explicit SignalChannelImpl(const CString& label, double frequency, double physicalMinimum, double physicalMaximum,
	                           const CString& dimension, size_t bufferSize, const CString& sensor = _T(""));
	virtual ~SignalChannelImpl();
	virtual const CString& getLabel() const;
	virtual void setLabel(const CString& label);
	virtual const CString& getSensor() const;
	virtual void setSensor(const CString& sensor);
	virtual const CString& getPhysicalDimension();
	virtual void setPhysicalDimension(const CString& dimension);
	virtual double getPhysicalMinimum() const;
	virtual void setPhysicalMinimum(double physicalMinimum);
	virtual double getPhysicalMaximum() const;
	virtual long getDigitalMinimum() const;
	virtual void setDigitalMinimum(long digitalMinimum);
	virtual long getDigitalMaximum() const;
	virtual void setDigitalMaximum(long digitalMaximum);
	virtual double getHighPassFilter() const;
	virtual void setHighPassFilter(double highPass);
	virtual double getLowPassFilter() const;
	virtual void setLowPassFilter(double lowPass);
	virtual double getNotchFilter() const;
	virtual void setNotchFilter(double notch);
	virtual double getSampleFrequency() const;
	virtual void setSampleFrequency(double sampleFrequency);
	virtual const Annotations& getAnnotations() const;
	virtual Annotations& getAnnotations();
	virtual const SignalBuffer<double>& getSignalBuffer() const;
	virtual SignalBuffer<double>& getSignalBuffer();
	virtual size_t getSampleCount() const;
	virtual void setPhysicalMaximum(double physicalMaximum);
private:
	CString _label;
	CString _sensor;
	CString _physicalDimension;
	double _physicalMinimum;
	double _physicalMaximum;
	long  _digitalMinimum;
	long  _digitalMaximum;
	double _highPassFilter;
	double _lowPassFilter;
	double _notchFilter;
	double _sampleFrequency;
	Annotations _annotations;
	SignalBuffer<double> _signalBuffer;
};

