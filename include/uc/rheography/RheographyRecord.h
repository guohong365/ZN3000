#pragma once
#include <uc/libzn.h>
#include <vector>

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


