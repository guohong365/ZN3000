#pragma once
#include "SignalBuffer.h"
#include <vector>

class SignalChannel;

class Annotation
{
public:
	virtual ~Annotation(){}
	virtual SIZE_T getOffset() CONST=0;
	virtual VOID setOffset(SIZE_T offset)=0;
	virtual CONST CString& getLabel() CONST=0;
	virtual VOID setLabel(CONST CString & label)=0;
	virtual const SignalChannel* getOwner() const=0;
	virtual SignalChannel * getOwner()=0;
	virtual void setOwner(SignalChannel * pChannel)=0;
};

typedef std::vector<Annotation *> Annotations;

class SignalChannel
{	
public:
	virtual ~SignalChannel(){}
	virtual CONST CString& getLabel() CONST=0;
	virtual VOID setLabel(CONST CString& label)=0;
	virtual CONST CString& getSensor() CONST=0;
	virtual VOID setSensor(CONST CString& sensor)=0;
	virtual CONST CString& getPhysicalDimension()=0;
	virtual VOID setPhysicalDimension(CONST CString & dimension)=0;
	virtual DOUBLE getPhysicalMinimum() CONST=0;
	virtual VOID setPhysicalMinimum(DOUBLE physicalMinimum)=0;
	virtual DOUBLE getPhysicalMaximum() CONST=0;
	virtual void setPhysicalMaximum(double physicalMaximum)=0;
	virtual long getDigitalMinimum() CONST=0;
	virtual VOID setDigitalMinimum(long digitalMinimum)=0;
	virtual long getDigitalMaximum() CONST=0;
	virtual VOID setDigitalMaximum(long digitalMaximum)=0;
	virtual DOUBLE getHighPassFilter() CONST=0;
	virtual VOID setHighPassFilter(DOUBLE highPass)=0;
	virtual DOUBLE getLowPassFilter() CONST=0;
	virtual VOID setLowPassFilter(DOUBLE lowPass)=0;
	virtual DOUBLE getNotchFilter() CONST=0;
	virtual VOID setNotchFilter(DOUBLE notch)=0;
	virtual DOUBLE getSampleFrequency() CONST=0;
	virtual VOID setSampleFrequency(DOUBLE sampleFrequency)=0;
	virtual SIZE_T getSampleCount() const=0;
	virtual CONST Annotations & getAnnotations() CONST=0;
	virtual Annotations & getAnnotations()=0;
	virtual CONST SignalBuffer<SHORT> &getSignalBuffer() CONST=0;
	virtual SignalBuffer<SHORT>& getSignalBuffer()=0;
};

typedef std::vector<SignalChannel *> SignalChannels;

class RheographyRecord
{
public:
	virtual ~RheographyRecord(){}
	virtual CONST CString& getRecordId() CONST=0;
	virtual VOID setRecordId(CONST CString& id)=0;
	virtual CONST CString& getHospital() CONST=0;
	virtual VOID setHospital(CONST CString& hospital)=0;
	virtual CONST CString& getDepartment() CONST=0;
	virtual VOID setDepartment(CONST CString& department)=0;
	virtual __time64_t getStartTime() const=0;
	virtual void setStartTime(__time64_t startDate)=0;

	virtual CONST CString &getPatientName() CONST=0;
	virtual VOID setPatientName(CONST CString& name)=0;
	virtual CONST CString &getPatientId() CONST=0;
	virtual VOID setPatientId(CONST CString& id)=0;
	virtual CONST CString &getBirthday() CONST=0;
	virtual VOID setBirthday(CONST CString& date)=0;
	virtual TCHAR getGender() CONST=0;
	virtual VOID setGender(TCHAR gender)=0;
	virtual INT getAge() CONST=0;
	virtual VOID setAge(INT age)=0;
	virtual FLOAT getHeight()CONST=0;
	virtual VOID setHeight(FLOAT height)=0;
	virtual FLOAT getWeight() CONST=0;
	virtual VOID setWeight(FLOAT weight)=0;
	virtual FLOAT getSystolicBloodPressure() CONST=0;
	virtual VOID setSystolicBloodPressure(FLOAT systolicBloodPressure)=0;
	virtual FLOAT getDiastolicBloodPressure() CONST=0;
	virtual VOID setDiastolicBloodPressure(FLOAT diastolicBloodPressure)=0;
	virtual FLOAT getHeartRate() CONST=0;
	virtual VOID setHeartRate(FLOAT hearBeatRate)=0;
	virtual FLOAT getElevation() CONST=0;
	virtual VOID setElevation(FLOAT elevation)=0;	
	virtual SignalChannels& getSignalChannels()=0;
	virtual const SignalChannels & getSignalChannels() const=0;
};


