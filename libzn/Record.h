#pragma once
#include "SignalBuffer.h"
#include <vector>

class SignalChannel;

class Annotation
{
public:
	virtual ~Annotation(){}
	virtual unsigned long getOffset() const=0;
	virtual void setOffset(unsigned long offset)=0;
	virtual const CString& getLabel() const=0;
	virtual void setLabel(const CString & label)=0;
	virtual const SignalChannel* getOwner() const=0;
	virtual SignalChannel * getOwner()=0;
	virtual void setOwner(SignalChannel * pChannel)=0;
};

typedef std::vector<Annotation *> Annotations;

class SignalChannel
{	
public:
	virtual ~SignalChannel(){}
	virtual const CString& getLabel() const=0;
	virtual void setLabel(const CString& label)=0;
	virtual const CString& getSensor() const=0;
	virtual void setSensor(const CString& sensor)=0;
	virtual const CString& getPhysicalDimension()=0;
	virtual void setPhysicalDimension(const CString & dimension)=0;
	virtual double getPhysicalMinimum() const=0;
	virtual void setPhysicalMinimum(double physicalMinimum)=0;
	virtual double getPhysicalMaximum() const=0;
	virtual void setPhysicalMaximum(double physicalMaximum)=0;
	virtual long getDigitalMinimum() const=0;
	virtual void setDigitalMinimum(long digitalMinimum)=0;
	virtual long getDigitalMaximum() const=0;
	virtual void setDigitalMaximum(long digitalMaximum)=0;
	virtual double getHighPassFilter() const=0;
	virtual void setHighPassFilter(double highPass)=0;
	virtual double getLowPassFilter() const=0;
	virtual void setLowPassFilter(double lowPass)=0;
	virtual double getNotchFilter() const=0;
	virtual void setNotchFilter(double notch)=0;
	virtual double getSampleFrequency() const=0;
	virtual void setSampleFrequency(double sampleFrequency)=0;
	virtual unsigned long getSampleCount() const=0;
	virtual const Annotations & getAnnotations() const=0;
	virtual Annotations & getAnnotations()=0;
	virtual const SignalBuffer<unsigned short> &getSignalBuffer() const=0;
	virtual SignalBuffer<unsigned short>& getSignalBuffer()=0;
};

typedef std::vector<SignalChannel *> SignalChannels;

class RheographyRecord
{
public:
	virtual ~RheographyRecord(){}
	virtual const CString& getRecordId() const=0;
	virtual void setRecordId(const CString& id)=0;
	virtual const CString& getHospital() const=0;
	virtual void setHospital(const CString& hospital)=0;
	virtual const CString& getDepartment() const=0;
	virtual void setDepartment(const CString& department)=0;
	virtual __time64_t getStartTime() const=0;
	virtual void setStartTime(__time64_t startDate)=0;

	virtual const CString &getPatientName() const=0;
	virtual void setPatientName(const CString& name)=0;
	virtual const CString &getPatientId() const=0;
	virtual void setPatientId(const CString& id)=0;
	virtual const CString &getBirthday() const=0;
	virtual void setBirthday(const CString& date)=0;
	virtual TCHAR getGender() const=0;
	virtual void setGender(TCHAR gender)=0;
	virtual INT getAge() const=0;
	virtual void setAge(INT age)=0;
	virtual FLOAT getHeight()const=0;
	virtual void setHeight(FLOAT height)=0;
	virtual FLOAT getWeight() const=0;
	virtual void setWeight(FLOAT weight)=0;
	virtual FLOAT getSystolicBloodPressure() const=0;
	virtual void setSystolicBloodPressure(FLOAT systolicBloodPressure)=0;
	virtual FLOAT getDiastolicBloodPressure() const=0;
	virtual void setDiastolicBloodPressure(FLOAT diastolicBloodPressure)=0;
	virtual FLOAT getHeartRate() const=0;
	virtual void setHeartRate(FLOAT hearBeatRate)=0;
	virtual FLOAT getElevation() const=0;
	virtual void setElevation(FLOAT elevation)=0;	
	virtual SignalChannels& getSignalChannels()=0;
	virtual const SignalChannels & getSignalChannels() const=0;
};


