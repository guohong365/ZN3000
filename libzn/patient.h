#pragma once

class IPatient
{
public:
	virtual ~IPatient();;
	virtual const CString &getName()=0;
	virtual void setName(const CString& name)=0;
	virtual const CString &getId()=0;
	virtual void setId(const CString& id)=0;
	virtual const CString &getBirthday()=0;
	virtual void setBirthday(const CString& date)=0;
	virtual TCHAR getGender()=0;
	virtual void setGender(TCHAR gender)=0;
	virtual int getAge()=0;
	virtual void setAge(int age)=0;
	virtual float getHeight()=0;
	virtual void setHeight(float height)=0;
	virtual float getWeight()=0;
	virtual void setWeight(float weight)=0;
	virtual float getSystolicBloodPressure()=0;
	virtual void setSystolicBloodPressure(float systolicBloodPressure)=0;
	virtual float getDiastolicBloodPressure()=0;
	virtual void setDiastolicBloodPressure(float diastolicBloodPressure)=0;
	virtual float getHeartRate()=0;
	virtual void setHeartRate(float hearBeatRate)=0;
	virtual float getElevation()=0;
	virtual void setElevation(float elevation)=0;
};

inline IPatient::~IPatient() {}

