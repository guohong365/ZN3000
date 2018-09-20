#pragma once
#include "Record.h"

class SignalChannelImpl : public SignalChannel
{
public:
	explicit SignalChannelImpl(SIZE_T bufferSize);
	virtual ~SignalChannelImpl();
	virtual const CString& getLabel() const;
	virtual void setLabel(const CString& label);
	virtual const CString& getSensor() const;
	virtual void setSensor(const CString& sensor);
	virtual const CString& getPhysicalDimension();
	virtual void setPhysicalDimension(const CString& dimension);
	virtual DOUBLE getPhysicalMinimum() const;
	virtual void setPhysicalMinimum(DOUBLE physicalMinimum);
	virtual DOUBLE getPhysicalMaximum() const;
	virtual long getDigitalMinimum() const;
	virtual void setDigitalMinimum(long digitalMinimum);
	virtual long getDigitalMaximum() const;
	virtual void setDigitalMaximum(long digitalMaximum);
	virtual DOUBLE getHighPassFilter() const;
	virtual void setHighPassFilter(DOUBLE highPass);
	virtual DOUBLE getLowPassFilter() const;
	virtual void setLowPassFilter(DOUBLE lowPass);
	virtual DOUBLE getNotchFilter() const;
	virtual void setNotchFilter(DOUBLE notch);
	virtual DOUBLE getSampleFrequency() const;
	virtual void setSampleFrequency(DOUBLE sampleFrequency);
	virtual const Annotations& getAnnotations() const;
	virtual Annotations& getAnnotations();
	virtual const SignalBuffer<SHORT>& getSignalBuffer() const;
	virtual SignalBuffer<SHORT>& getSignalBuffer();
	virtual SIZE_T getSampleCount() const;
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
	SignalBuffer<short> _signalBuffer;
};

